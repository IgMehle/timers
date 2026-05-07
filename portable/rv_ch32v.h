/**
 * rv_ch32v.h
 *
 *  Created on: 6 may. 2026
 *      Author: Ignacio Mehle
 */
#ifndef TIMERS_PORTABLE_RV_CH32V_H_
#define TIMERS_PORTABLE_RV_CH32V_H_
 
#include "../timers_config.h"

/* ====== DEVICE HEADER ====== */
#include DEVICE_HEADER

/* ====== CORE HEADER ====== */
#include TIMERS_CORE_HEADER

/* ===== TIMER IRQ ===== */
#define TIMERS_DISABLE_TIMER_IRQ() 
#define TIMERS_ENABLE_TIMER_IRQ() 

/* ===== GENERAL IRQ ===== */
#define TIMERS_DISABLE_ALL_IRQ()    __asm volatile("csrci mstatus, 8")
#define TIMERS_ENABLE_ALL_IRQ()     __asm volatile("csrsi mstatus, 8")

/* ====== IRQ LOCK ====== */
#if TIMER_IRQ_ONLY_LOCK            
    #define TIMERS_IRQ_LOCK()           TIMERS_DISABLE_TIMER_IRQ()
    #define TIMERS_IRQ_UNLOCK()         TIMERS_ENABLE_TIMER_IRQ()
#else
    #define TIMERS_IRQ_LOCK()           TIMERS_DISABLE_ALL_IRQ()
    #define TIMERS_IRQ_UNLOCK()         TIMERS_ENABLE_ALL_IRQ()
#endif

/* ====== ATOMICIDAD ====== */
#define TIMERS_ATOMIC_8                 (1)
#define TIMERS_ATOMIC_16                (1)
// si es RV32
#define TIMERS_ATOMIC_32                (1) 

/* ====== DEFERRED ISR ====== */
#define TIMERS_USE_DEFERRED_ISR     (0)

#endif /* TIMERS_PORTABLE_RV_CH32V_H_ */