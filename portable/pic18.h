/**
 * pic18.h
 *
 *  Created on: 26 apr. 2026
 *      Author: Ignacio Mehle
 */
#ifndef TIMERS_PORTABLE_PIC18_H_
#define TIMERS_PORTABLE_PIC18_H_

#include "../timers_config.h"

/* ====== DEVICE HEADER ====== */
#include DEVICE_HEADER

/* ====== CORE HEADER ====== */
#include TIMERS_CORE_HEADER

/* ===== TIMER IRQ ===== */
#define TIMERS_DISABLE_TIMER_IRQ() 
#define TIMERS_ENABLE_TIMER_IRQ() 

/* ===== GENERAL IRQ ===== */
#define TIMERS_DISABLE_ALL_IRQ()    INTCONbits.GIE = 0
#define TIMERS_ENABLE_ALL_IRQ()     INTCONbits.GIE = 1

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
#define TIMERS_ATOMIC_16                (0)
#define TIMERS_ATOMIC_32                (0)

/* ====== DEFERRED ISR ====== */
#define TIMERS_USE_DEFERRED_ISR     (0)

#endif /* TIMERS_PORTABLE_PIC18_H_ */