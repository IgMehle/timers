/**
 * pic18.h
 *
 *  Created on: 26 apr. 2026
 *      Author: Ignacio Mehle
 */
#include "../timers_config.h"

/* ====== DEVICE HEADER ====== */
#include DEVICE_HEADER

/* ====== CORE HEADER ====== */

/* ====== SECCION CRITICA ====== */
#define TIMERS_CRITICAL_ENTER()     INTCONbits.GIE = 0
#define TIMERS_CRITICAL_EXIT()      INTCONbits.GIE = 1

/* ====== DEFERRED ISR ====== */
#define TIMERS_USE_DEFERRED_ISR     (0)

/* ====== OPTIMIZACIONES ====== */
#define TIMERS_ATOMIC_32            (0)