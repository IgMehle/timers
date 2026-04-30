/**
 * timers.c
 *
 *  Created on: 1 feb. 2026
 *      Author: Ignacio Mehle
 */

 /* -----------------------------------------------------
 * HEADERS
 * -----------------------------------------------------*/
#include "timers.h"

/* -----------------------------------------------------
 * GLOBAL VARIABLES
 * -----------------------------------------------------*/
static timer_t *timers = 0;
static uint8_t n_timers = 0;
static uint8_t timers_count = 0;
static volatile uint16_t timers_prescaler_counter = TIMERS_PRESCALER_VALUE;

#if USE_QUEUES
static timers_queue_t timers_queues[N_TIMER_PRIORITIES];
#endif

/* -----------------------------------------------------
 * INIT / CONFIG
 * -----------------------------------------------------*/
void timers_init(timer_t *my_timers, uint8_t n)
{
    timers = my_timers;
    if (n < MAX_N_TIMERS) n_timers = n;
	else n_timers = MAX_N_TIMERS;

#if USE_QUEUES
    // inicializo timers queues
    for (uint8_t p = 0; p < N_PRIORITIES; p++) {
        timers_queues[p].head = 0;
        timers_queues[p].tail = 0;
    }
#endif
}

uint8_t give_timer(counter_t reload, uint8_t priority, GIVE_TIMER_ARGS)
{
	uint8_t n_timer = 0xFF;
    uint8_t flags = 0x00;
	// Si no llegue al maximo de timers, agrego
	if (timers_count < n_timers) {
		// Lleno los campos del nuevo timer
        // Default flags: 0b00000000
        // SETEO PRIORIDAD
        if (priority < N_TIMER_PRIORITIES) {
            flags_set_priority(&flags, priority);
        }
        else {
            // si paso priority = TIMER_CRITICAL
            // y no tengo activado USE_TIMER_CRITICAL
            // coloco maxima prioridad por default
            if (priority == TIMER_CRITICAL) {   
#if USE_TIMER_CRITICAL
                flags_set(&flags, FLAG_CRITICAL);      
#endif
                flags_set_priority(&flags, 0);
            }
            // clamp contra overflow
            else flags_set_priority(&flags, (N_TIMER_PRIORITIES-1));
        }
        timers[timers_count].flags =    flags;
		timers[timers_count].rep =      PERIODIC_TIMER;
		timers[timers_count].reload =   reload;
		timers[timers_count].ticks =    reload;
		timers[timers_count].callback = callback;
#if USE_CALLBACK_CONTEXT
        timers[timers_count].context = context;
#endif
		// Devuelvo el numero de timer
		n_timer = timers_count;
		// Incremento la cantidad de timers creados
		timers_count++;
	}
	return n_timer;
}

/* -----------------------------------------------------
 * TIMER MANIPULATION
 * -----------------------------------------------------*/
void on_timer(uint8_t id, uint8_t rep)
{
	timers[id].ticks = timers[id].reload;
	if (rep != 0) timers[id].rep = rep;
	flags_set(&timers[id].flags, FLAG_ENABLED);
}

void pause_timer(uint8_t id)
{
	flags_clear(&timers[id].flags, FLAG_ENABLED);
}

void continue_timer(uint8_t id)
{
    flags_set(&timers[id].flags, FLAG_ENABLED);
}

void reload_timer(uint8_t id)
{
	timers[id].ticks = timers[id].reload;
}

void set_timer_priority(uint8_t id, uint8_t priority)
{   
    // SETEO PRIORIDAD
    if (priority < N_TIMER_PRIORITIES) {
        flags_set_priority(&timers[id].flags, priority);
    }
    else {
        // si paso priority == TIMER_CRITICAL
        // y no tengo activado USE_TIMER_CRITICAL
        // coloco maxima prioridad por default
        if (priority == TIMER_CRITICAL) {
#if USE_TIMER_CRITICAL
            flags_set(&timers[id].flags, FLAG_CRITICAL);      
#endif
            flags_set_priority(&timers[id].flags, 0);
        }
        // clamp contra overflow
        else flags_set_priority(&timers[id].flags, (N_TIMER_PRIORITIES-1));
    }
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

void set_timer_prescaler(uint8_t id)
{
    flags_set(&timers[id].flags, FLAG_PRESCALER);
}

void clear_timer_prescaler(uint8_t id)
{
    flags_clear(&timers[id].flags, FLAG_PRESCALER);
}

void resize_timer(uint8_t id, counter_t ticks)
{
	timers[id].reload = ticks;
	timers[id].ticks = ticks;
}

void off_timer(uint8_t id)
{
	flags_clear(&timers[id].flags, FLAG_ENABLED);
	timers[id].ticks = timers[id].reload;
}

timer_t get_timer_status(uint8_t id)
{
    timer_t tmr;
    tmr.flags =     timers[id].flags;
    tmr.rep =       timers[id].rep;
    tmr.reload =    timers[id].reload;
    tmr.ticks =     timers[id].ticks;
    tmr.callback =  timers[id].callback;
#if USE_CALLBACK_CONTEXT
    tmr.context = timers[id].context;
#endif
//#if USE_QUEUES == 0
//    tmr.event =     timers[id].event;
//#endif
    return tmr;
}

/* -----------------------------------------------------
 * TIMERS CORE
 * -----------------------------------------------------*/
void timers_tick(void)
{
    // Decremento contador de prescaler por cada tick
    timers_prescaler_counter--;

	// itero por todos los timers creados
	for (uint8_t i = 0; i < timers_count; i++) {
		// si el timer esta habilitado
		if (flags_get(timers[i].flags, FLAG_ENABLED)) {
			// si vencio el contador
			if (timers[i].ticks == 0) {
                // Si el timer esta marcado como critical
                // Y esta activo USE_TIMER_CRITICAL
                // llamo al callback
                if (flags_get(timers[i].flags, FLAG_CRITICAL)) {
#if USE_TIMER_CRITICAL
                    uint8_t callback_status = CALLBACK_OK;
                    callback_status = TIMER_INVOKE_CALLBACK(&timers[i]);
#endif
                }
                // si el timer NO esta marcado como CRITICAL
                // lo encolo / levanto flag de pending
                else {
#if USE_QUEUES
                    // encolo timer
                    push_timers_queue(i, flags_get_priority(timers[i].flags));
#else
                    // levanto el flag de pending
				    flags_set(&timers[i].flags, FLAG_PENDING);
#endif
                }
    			// recargo el contador
                timers[i].ticks = timers[i].reload;

				// si el timer NO ES PERIODICO, decremento repeticiones
				if (timers[i].rep != PERIODIC_TIMER) {
                    if (timers[i].rep != 0) timers[i].rep--;
                    // si se acabaron las repeticiones, lo deshabilito
                    if (timers[i].rep == 0) flags_clear(&timers[i].flags, FLAG_ENABLED);
                }
			}
			// si el contador no esta en cero
			else {
                // Si el prescaler del timer esta activo
                if (flags_get(timers[i].flags, FLAG_PRESCALER)) {
                    // Si el valor de timers_prescaler_counter esta en 0
                    // Decremento un tick del timer
                    if (timers_prescaler_counter == 0) timers[i].ticks--;
                }
                // Si no tiene prescaler, decremento ticks normalmente
				else timers[i].ticks--;
			}
		}
	}

    if (timers_prescaler_counter == 0) {
        timers_prescaler_counter = TIMERS_PRESCALER_VALUE;
    }
}

uint8_t push_timers_queue(uint8_t id, uint8_t priority)
{
    // puntero a queue segun prioridad del timer
    timers_queue_t *q = &timers_queues[priority];
    // calculo la posicion del nuevo head
    uint8_t next_head = (q->head + 1) & TIMERS_QUEUE_MASK;
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

uint8_t pop_timers_queue(uint8_t *id, uint8_t priority)
{
    // puntero a queue segun prioridad del timer
    timers_queue_t *q = &timers_queues[priority];
    // si head == tail, la cola esta vacia
    if (q->head == q->tail) return QUEUE_NACK;
    // leo el numero de timer de la cola
    *id = q->bf[q->tail];
    // actualizo tail
    q->tail = (q->tail + 1) & TIMERS_QUEUE_MASK;
    return QUEUE_ACK;
}

void timers_process(uint8_t priority)
{
    uint8_t callback_status = CALLBACK_OK;

#if USE_QUEUES
    uint8_t i;
    // vacio la queue de timers de la prioridad especifica
    while (pop_timers_queue(&i, priority)) {
        // llamo al callback
        callback_status = TIMER_INVOKE_CALLBACK(&timers[i]);
    }
#else
    // itero por todos los timers creados
	for (uint8_t i = 0; i < timers_count; i++) {
        // si el flag de pending esta habilitado
        // y el timer tiene la prioridad especificada
        if (flags_get(timers[i].flags, FLAG_PENDING)) {
            if (flags_get_priority(timers[i].flags) == priority) {
                // llamo al callback
                callback_status = TIMER_INVOKE_CALLBACK(&timers[i]);
                // limpio flag de pending
                flags_clear(&timers[i].flags, FLAG_PENDING);
            }
        }
    }
#endif
}
/* ----------------------------------------------------- */