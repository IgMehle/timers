/**
 * timers.h
 *
 *  Created on: 1 feb. 2026
 *      Author: Ignacio Mehle
 */

#ifndef TIMERS_H_
#define TIMERS_H_

/* ===== INCLUDES ===== */
#include <stdint.h>
#include "timers_config.h"
#include "portable/timers_port_select.h"

/* ===== TIMER_T ===== */
typedef struct timer {
    uint8_t             priority;
	volatile uint8_t    enabled;
#if USE_QUEUES == 0
	volatile uint8_t    event;
#endif
    volatile uint8_t    rep;
	uint8_t             (*callback)(void);
    uint32_t            reload;
	volatile uint32_t   ticks;
} timer_t;

/* ===== TIMERS_QUEUE_T ===== */
typedef struct timers_queue {
    volatile uint8_t    bf[QUEUE_SIZE];
    // escribe la ISR (timers_tick)
    volatile uint8_t    head;
    // lee el consumidor (timers_process)
    volatile uint8_t    tail;
    // flags
    // volatile uint8_t empty;
    // volatile uint8_t full;
} timers_queue_t;

/* ===== DEFINICIONES ===== */
#define EVENT_PENDING   1U
#define EVENT_CLEAR     0U
#define MAX_PRIORITIES  8U
#define TIMER_PERIODIC	0xFF
#define TIMER_MAX_REP   0xFE
#define CALLBACK_OK     0U
#define QUEUE_NACK      0U
#define QUEUE_ACK       1U

/* ===== INIT / CONFIG ===== */
void timers_init(timer_t *my_timers, uint8_t n);
uint8_t give_timer(uint32_t reload, uint8_t (*callback)(), uint8_t priority);

/* ===== TIMER MANIPULATION ===== */
void on_timer(uint8_t id, uint8_t rep);
void pause_timer(uint8_t id);
void continue_timer(uint8_t id);
void reload_timer(uint8_t id);
void set_timer_priority(uint8_t id, uint8_t priority);
void set_timer_repeats(uint8_t id, uint8_t rep);
void add_timer_repeats(uint8_t id, uint8_t rep);
void resize_timer(uint8_t id, uint32_t time);
void off_timer(uint8_t id);
timer_t get_timer_status(uint8_t id);

/* ===== TIMERS CORE ===== */
void timers_tick(void);
uint8_t push_timers_queue(uint8_t id, uint8_t priority);
uint8_t pop_timers_queue(uint8_t *id, uint8_t priority);
void timers_process(uint8_t priority);

#endif /* TIMERS_H_ */