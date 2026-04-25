/*
 * timers.h
 *
 *  Created on: 1 feb. 2026
 *      Author: Ignacio Mehle
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#include <stdint.h>

// TIMER_T
typedef struct timer {
	uint8_t name;
	uint8_t enabled;
	uint8_t priority;
	uint8_t event;
    uint8_t rep;
	uint8_t (*callback)(void);
    uint32_t time;
	uint32_t count;
} timer_t;

#define N_TIMERS		    16U
#define MAX_N_TIMERS        64U
#define TIMER_EVENT_ON      1U
#define TIMER_EVENT_OFF     0U
#define TIMER_PERIODIC	    0xFF
#define TIMER_MAX_REP       0xFE
#define TIMER_CALLBACK_OK   0U

// INIT - CONFIG
void init_timers(void);
uint8_t give_timer(uint32_t time, uint8_t (*callback)(), uint8_t priority)
// TIMER MANIPULATION
void on_timer(uint8_t name, uint8_t rep);
void pause_timer(uint8_t name);
void continue_timer(uint8_t name);
void reset_timer(uint8_t name);
void set_timer_priority(uint8_t name, uint8_t priority);
void set_timer_repeats(uint8_t name, uint8_t rep);
void add_timer_repeats(uint8_t name, uint8_t rep);
void resize_timer(uint8_t name, uint32_t time);
void off_timer(uint8_t name);
timer_t get_timer_status(uint8_t name);
// TIMERS CORE
void timers_tick(void);
void timers_process(uint8_t priority);

#endif /* TIMERS_H_ */
