/**
 * timers.h
 *
 *  Created on: 1 feb. 2026
 *      Author: Ignacio Mehle
 */

#ifndef TIMERS_H_
#define TIMERS_H_

/* ===== INCLUDES ===== */
#include "timers_config.h"
#include "portable/timers_port_select.h"

/* ===== DEFINICIONES ===== */
#define EVENT_PENDING   (1)
#define EVENT_CLEAR     (0)
#define MAX_PRIORITIES  (8)
#define PERIODIC_TIMER	(0xFF)
#define TIMER_MAX_REP   (0xFE)
#define CALLBACK_OK     (0)
#define QUEUE_NACK      (0)
#define QUEUE_ACK       (1)
#define TIMER_CRITICAL  (0xFF)

/* ===== COUNTERS SIZE ===== */
#if USE_16BIT_COUNTERS
typedef uint16_t    counter_t  
#else
typedef uint32_t    counter_t
#endif

/* ===== TIMER BYTE FLAGS ===== */
// 0: enabled
// 1: pending (event)
// 2: critical
// 3: prescaler
// 4: RESERVED (watchdog_expired)
// 5: priority.b0
// 6: priority.b1
// 7: priority.b2
#define FLAG_ENABLED    (1 << 0)
#define FLAG_PENDING    (1 << 1)
#define FLAG_CRITICAL   (1 << 2)
#define FLAG_PRESCALER  (1 << 3)
#define FLAG_WD_EXPIRED (1 << 4)
#define PRIORITY_MASK   (0xE0)
#define PRIORITY_SHIFT  (5)

/* ===== FLAGS INLINES ===== */
static inline uint8_t flags_get(uint8_t flags, uint8_t mask) {
    return (flags & mask);
}
static inline void flags_set(uint8_t *flags, uint8_t mask) {
    *flags |= mask;
}
static inline void flags_clear(uint8_t *flags, uint8_t mask) {
    *flags &= ~mask;
}
static inline uint8_t flags_get_priority(uint8_t flags) {
    return (flags & PRIORITY_MASK) >> PRIORITY_SHIFT;
}
static inline void flags_set_priority(uint8_t *flags, uint8_t priority) {
    priority &= 0x07;
    *flags &= ~PRIORITY_MASK;
    *flags |= (priority << PRIORITY_SHIFT);
}

// Callback typedef
typedef TIMER_CALLBACK_RET_TYPE (*timer_callback_t)(TIMER_CALLBACK_ARGS);

/* ===== TIMER_T ===== */
typedef struct {
    volatile uint8_t    flags;
//    uint8_t             priority;
//	volatile uint8_t    enabled;
//#if USE_QUEUES == 0
//	volatile uint8_t    event;
//#endif
    volatile uint8_t    rep;
    counter_t           reload;
    volatile counter_t  ticks;
    timer_callback_t    callback;
#if USE_CALLBACK_CONTEXT
    void                *context;
#endif
} timer_t;

// Macro de llamada al callback
#if USE_CALLBACK_CONTEXT
    #define TIMER_INVOKE_CALLBACK(timer) (timer)->callback((timer)->context)
#else
    #define TIMER_INVOKE_CALLBACK(timer) (timer)->callback()
#endif

// Argumentos de give_timer()
#if USE_CALLBACK_CONTEXT
    #define GIVE_TIMER_ARGS timer_callback_t callback, void *context
#else
    #define GIVE_TIMER_ARGS timer_callback_t callback
#endif

/* ===== TIMERS_QUEUE_T ===== */
typedef struct {
    volatile uint8_t    bf[TIMERS_QUEUE_SIZE];
    // escribe la ISR (timers_tick)
    volatile uint8_t    head;
    // lee el consumidor (timers_process)
    volatile uint8_t    tail;
    // flags
    // volatile uint8_t empty;
    // volatile uint8_t full;
} timers_queue_t;

/* ===== INIT / CONFIG ===== */
void timers_init(timer_t *my_timers, uint8_t n);
uint8_t give_timer(counter_t reload, uint8_t priority, GIVE_TIMER_ARGS);

/* ===== TIMER MANIPULATION ===== */
void on_timer(uint8_t id, uint8_t rep);
void pause_timer(uint8_t id);
void continue_timer(uint8_t id);
void reload_timer(uint8_t id);
void set_timer_priority(uint8_t id, uint8_t priority);
void set_timer_repeats(uint8_t id, uint8_t rep);
void add_timer_repeats(uint8_t id, uint8_t rep);
void set_timer_prescaler(uint8_t id);
void clear_timer_prescaler(uint8_t id);
void resize_timer(uint8_t id, counter_t ticks);
void off_timer(uint8_t id);
timer_t get_timer_status(uint8_t id);

/* ===== TIMERS CORE ===== */
void timers_tick(void);
uint8_t push_timers_queue(uint8_t id, uint8_t priority);
uint8_t pop_timers_queue(uint8_t *id, uint8_t priority);
void timers_process(uint8_t priority);

#endif /* TIMERS_H_ */