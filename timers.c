/*
 * timers.c
 *
 *  Created on: 1 feb. 2026
 *      Author: Ignacio Mehle
 */

#include "timers.h"
#include "timers_config.h"

/* -----------------------------------------------------
 * GLOBAL VARIABLES
 * -----------------------------------------------------*/
static timer_t *timers = NULL;
static uint8_t n_timers = 0;
static uint8_t timers_count = 0;
#if USE_QUEUES == 1
static timers_queue_t timers_queues[N_PRIORITIES];
#endif

/* -----------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------*/
void timers_init(timer_t *my_timers, uint8_t n)
{
    timers = my_timers;
    if (n < MAX_N_TIMERS) n_timers = n;
	else n_timers = MAX_N_TIMERS;

#if USE_QUEUES == 1
    // inicializo timers queues
    for (uint8_t p = 0; p < N_PRIORITIES; p++) {
        timers_queues[p].head = 0;
        timers_queues[p].tail = 0;
    }
#endif
}

uint8_t give_timer(uint32_t time, uint8_t (*callback)(), uint8_t priority)
{
	uint8_t n_timer = 0xFF;
	// Si no llegue al maximo de timers, agrego
	if (timers_count < n_timers) {
		// Lleno los campos del nuevo timer
		// Inicia apagado
		timers[timers_count].id =       timers_count;
        timers[timers_count].priority = priority;
		timers[timers_count].enabled =  0;
#if USE_QUEUES == 0
        timers[timers_count].event =    0;
#endif
		timers[timers_count].rep =      TIMER_PERIODIC;
        timers[timers_count].callback = callback;
		timers[timers_count].time =     time;
		timers[timers_count].counter =  time;
		// Devuelvo el numero de timer
		n_timer = timers[timers_count].id;
		// Incremento la cantidad de timers creados
		timers_count++;
	}
	return n_timer;
}

void on_timer(uint8_t id, uint8_t rep)
{
	timers[id].counter = timers[id].time;
	if (rep != 0) timers[id].rep = rep;
	timers[id].enabled = 1;
}

void pause_timer(uint8_t id)
{
	timers[id].enabled = 0;
}

void continue_timer(uint8_t id)
{
	timers[id].enabled = 1;
}

void reset_timer(uint8_t id)
{
	timers[id].counter = timers[id].time;
}

void set_timer_priority(uint8_t id, uint8_t priority)
{
    if (priority < N_PRIORITIES) {
        timers[id].priority = priority;
    }
    else timers[id].priority = N_PRIORITIES - 1;
}

void set_timer_repeats(uint8_t id, uint8_t rep)
{
    if (rep != 0) timers[id].rep = rep;
}

void add_timer_repeats(uint8_t id, uint8_t rep)
{
    uint16_t sum_rep = rep + timers[id].rep;
    if (rep != 0 && sum_rep != 0) {
        if (sum_rep > TIMER_MAX_REP) timers[id].rep = TIMER_MAX_REP;
        else timers[id].rep = (uint8_t) sum_rep;
    }
}

void resize_timer(uint8_t id, uint32_t time)
{
	timers[id].time = time;
	timers[id].counter = time;
}

void off_timer(uint8_t id)
{
	timers[id].enabled = 0;
	timers[id].counter = timers[id].time;
}

timer_t get_timer_status(uint8_t id)
{
    timer_t tmr;
    tmr.id =        timers[id].id;
    tmr.priority =  timers[id].priority;
    tmr.enabled =   timers[id].enabled;
    tmr.rep =       timers[id].rep;
#if USE_QUEUES == 0
    tmr.event =     timers[id].event;
#endif
    tmr.time =      timers[id].time;
    tmr.counter =   timers[id].counter;
    tmr.callback =  timers[id].callback;
    return tmr;
}

void timers_tick(void)
{
	// Itero por todos los timers creados
	for (uint8_t i = 0; i < timers_count; i++) {
		// Si el timer esta habilitado
		if (timers[i].enabled) {
			// Si vencio el contador
			if (timers[i].counter == 0) {
#if USE_QUEUES == 0
                // levanto el flag de event
				timers[i].event = 1;
#endif
#if USE_QUEUES == 1
                push_timers_queue(i);
#endif
#if TIMER_CRITICAL_ENABLED == 1
                uint8_t callback_status = CALLBACK_OK;
                callback_status = timers[i].callback();
#endif
				// si el timer es periodico, solo recargo el contador
				if (timers[i].rep == TIMER_PERIODIC) {
                    timers[i].counter = timers[i].time;
                }
				// si NO ES PERIODICO, decremento repeticiones
				else {
                    if (timers[i].rep != 0) timers[i].rep--;
                    // si se acabaron las repeticiones, lo deshabilito
                    if (timers[i].rep == 0) timers[i].enabled = 0;
                }
			}
			// si el contador no esta en cero, decremento
			else {
				timers[i].counter--;
			}
		}
	}
}

uint8_t push_timers_queue(uint8_t id)
{
    // puntero a queue segun prioridad del timer
    timers_queue_t *q = &timers_queues[timers[id].priority];
    // calculo la posicion del nuevo head
    uint8_t next_head = (q->head + 1) & QUEUE_MASK;
    // si next apunta al tail, la cola esta llena
    if (next_head == q->tail) {
        // overflow
        return QUEUE_NACK;
    }
    // encolo timer 
    q->bf[q->head] = id;
    // actualizo head
    q->head = next_head;
    return QUEUE_ACK;
}

uint8_t pop_timers_queue(uint8_t priority, uint8_t *id)
{
    // puntero a queue segun prioridad del timer
    timers_queue_t *q = &timers_queues[priority];
    // si head == tail, la cola esta vacia
    if (q->head == q->tail) return QUEUE_NACK;
    // leo el numero de timer de la cola
    *id = q->bf[q->tail];
    // actualizo tail
    q->tail = (q->tail + 1) & QUEUE_MASK;
    return QUEUE_ACK;
}

void timers_process(uint8_t priority)
{
    uint8_t callback_status = CALLBACK_OK;
#if USE_QUEUES == 0
    // itero por todos los timers creados
	for (uint8_t i = 0; i < timers_count; i++) {
        // si el flag de event esta habilitado
        // y el timer tiene la prioridad especificada
        if (timers[i].event && timers[i].priority == priority) {
            // llamo al callback
            callback_status = timers[i].callback();
            // limpio flag de event
            timers[i].event = 0;
        }
    }
#endif
#if USE_QUEUES == 1
    uint8_t id;
    // vacio la queue de timers de la prioridad especifica
    while (pop_timers_queue(priority, &id)) {
        // llamo al callback
        callback_status = timers[id].callback();
    }
#endif
}
