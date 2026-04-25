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
	uint8_t id;
    uint8_t priority;
	volatile uint8_t enabled;
	volatile uint8_t pending;
    volatile uint8_t rep;
	uint8_t (*callback)(void);
    uint32_t time;
	volatile uint32_t count;
} timer_t;

#define MAX_N_TIMERS        64U
#define TIMER_PENDING_ON    1U
#define TIMER_PENDING_OFF   0U
#define TIMER_PERIODIC	    0xFF
#define TIMER_MAX_REP       0xFE
#define TIMER_CALLBACK_OK   0U

// INIT - CONFIG
void timers_init(timer_t *my_timers, uint8_t n);
uint8_t give_timer(uint32_t time, uint8_t (*callback)(), uint8_t priority);
// TIMER MANIPULATION
void on_timer(uint8_t id, uint8_t rep);
void pause_timer(uint8_t id);
void continue_timer(uint8_t id);
void reset_timer(uint8_t id);
void set_timer_priority(uint8_t id, uint8_t priority);
void set_timer_repeats(uint8_t id, uint8_t rep);
void add_timer_repeats(uint8_t id, uint8_t rep);
void resize_timer(uint8_t id, uint32_t time);
void off_timer(uint8_t id);
timer_t get_timer_status(uint8_t id);
// TIMERS CORE
void timers_tick(void);
void push_timers_queue(uint8_t id);
void pop_timers_queue(uint8_t id);
void timers_process(uint8_t priority);

#endif /* TIMERS_H_ */
