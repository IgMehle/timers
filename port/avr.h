/*
 * avr.h
 *
 *  Created on: 26 apr. 2026
 *      Author: Ignacio Mehle
 */

#include <avr/interrupt.h>

/* ====== SECCION CRITICA ====== */
#define TIMERS_CRITICAL_ENTER()     cli()
#define TIMERS_CRITICAL_EXIT()      sei()

/* ====== DEFERRED ISR ====== */
#define TIMERS_USE_DEFERRED_ISR     (0)

/* ====== OPTIMIZACIONES ====== */
#define TIMERS_ATOMIC_32            (0)
