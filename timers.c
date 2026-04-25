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

/* -----------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------*/
void timers_init(timer_t *my_timers, uint8_t n)
{
    timers = my_timers;
    if (n < MAX_N_TIMERS) n_timers = n;
	else n_timers = MAX_N_TIMERS;
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
        timers[timers_count].pending =  0;
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
    tmr.pending =   timers[id].pending;
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
                // levanto el flag de pending
				timers[i].pending = 1;
				// Si el timer es periodico, solo recargo el contador
				if (timers[i].rep == TIMER_PERIODIC) {
                    timers[i].counter = timers[i].time;
                }
				// Si NO ES PERIODICO, decremento repeticiones
				else {
                    if (timers[i].rep != 0) timers[i].rep--;
                    // si se acabaron las repeticiones, lo deshabilito
                    if (timers[i].rep == 0) timers[i].enabled = 0;
                }
			}
			// Si el contador no esta en cero, decremento
			else {
				timers[i].counter--;
			}
		}
	}
}

void push_timers_queue(uint8_t id)
{

}

void pop_timers_queue(uint8_t id)
{

}

void timers_process(uint8_t priority)
{
    uint8_t callback_status = TIMER_CALLBACK_OK;
    // Itero por todos los timers creados
	for (uint8_t i = 0; i < timers_count; i++) {
        // Si el flag de pending esta habilitado
        // y el timer tiene la prioridad especificada
        if (timers[i].pending && timers[i].priority == priority) {
            // Llamo al callback
            callback_status = timers[i].callback();
            // limpio flag de pending
            timers[i].pending = 0;
        }
    }
}
