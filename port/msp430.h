/*
 * msp430.h
 *
 *  Created on: 26 apr. 2026
 *      Author: Ignacio Mehle
 */

/* ====== SECCION CRITICA ====== */
#define TIMERS_CRITICAL_ENTER()     __disable_interrupt()
#define TIMERS_CRITICAL_EXIT()      __enable_interrupt()

/* ====== DEFERRED ISR ====== */
#define TIMERS_USE_DEFERRED_ISR     (0)

/* ====== OPTIMIZACIONES ====== */
#define TIMERS_ATOMIC_32            (0)