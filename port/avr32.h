/*
 * avr32.h
 *
 *  Created on: 26 apr. 2026
 *      Author: Ignacio Mehle
 */

/* ====== SECCION CRITICA ====== */
#define TIMERS_CRITICAL_ENTER()     Disable_global_interrupt()
#define TIMERS_CRITICAL_EXIT()      Enable_global_interrupt()

/* ====== DEFERRED ISR ====== */
#define TIMERS_USE_DEFERRED_ISR     (0)

/* ====== OPTIMIZACIONES ====== */
// si es RV32
#define TIMERS_ATOMIC_32            (1)