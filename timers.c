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
volatile timer_t timers[N_TIMERS];
volatile uint8_t timers_count = 0;

/* -----------------------------------------------------
 * FUNCTIONS
 * -----------------------------------------------------*/
void init_timers(void)
{
	timers_count = 0;
}

uint8_t give_timer(uint32_t time, uint8_t (*callback)(), uint8_t priority)
{
	uint8_t n_timer = 0xFF;
	// Si no llegue al maximo de timers, agrego
	if (timers_count < N_TIMERS) {
		// Lleno los campos del nuevo timer
		// Inicia apagado
		timers[timers_count].name =     timers_count;
		timers[timers_count].enabled =  0;
        timers[timers_count].priority = priority;
		timers[timers_count].rep =      TIMER_PERIODIC;
        timers[timers_count].event =    0;
		timers[timers_count].time =     time;
		timers[timers_count].counter =  time;
		timers[timers_count].callback = callback;
		// Devuelvo el numero de timer
		n_timer = timers[timers_count].name;
		// Incremento la cantidad de timers creados
		timers_count++;
	}
	return n_timer;
}

void on_timer(uint8_t name, uint8_t rep)
{
	timers[name].counter = timers[name].time;
	if (rep != 0) timers[name].rep = rep;
	timers[name].enabled = 1;
}

void pause_timer(uint8_t name)
{
	timers[name].enabled = 0;
}

void continue_timer(uint8_t name)
{
	timers[name].enabled = 1;
}

void reset_timer(uint8_t name)
{
	timers[name].counter = timers[name].time;
}

void set_timer_repeats(uint8_t name, uint8_t rep)
{
    if (rep != 0) timers[name].rep = rep;
}

void add_timer_repeats(uint8_t name, uint8_t rep)
{
    uint16_t sum_rep = rep + timers[name].rep;
    if (rep != 0 && sum_rep != 0) {
        if (sum_rep > TIMER_MAX_REP) timers[name].rep = TIMER_MAX_REP;
        else timers[name].rep = (uint8_t) sum_rep;
    }
}

void resize_timer(uint8_t name, uint32_t time)
{
	timers[name].time = time;
	timers[name].counter = time;
}

void off_timer(uint8_t name)
{
	timers[name].enabled = 0;
	timers[name].counter = timers[name].time;
}

timer_t get_timer_status(uint8_t name)
{
    timer_t tmr;
    tmr.name =      timers[name].name;
    tmr.enabled =   timers[name].enabled;
    tmr.priority =  timers[name].priority;
    tmr.rep =       timers[name].rep;
    tmr.event =     timers[name].event;
    tmr.time =      timers[name].time;
    tmr.counter =   timers[name].counter;
    tmr.callback =  timers[name].callback;
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
                // levanto el flag de evento
				timers[i].event = 1;
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

void timers_process(uint8_t priority)
{
    uint8_t callback_status = TIMER_CALLBACK_OK;
    // Itero por todos los timers creados
	for (uint8_t i = 0; i < timers_count; i++) {
        // Si el flag de evento esta habilitado
        // y el timer tiene la prioridad especificada
        if (timers[i].event && timers[i].priority == priority) {
            // Llamo al callback
            callback_status = timers[i].callback();
            // limpio flag de evento
            timers[i].event = 0;
        }
    }
}

