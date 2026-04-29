/**
 * timers_config.h
 *
 *  Created on: 25 apr. 2026
 *      Author: Ignacio Mehle
 */

#ifndef TIMERS_CONFIG_H_
#define TIMERS_CONFIG_H_
#include <stdint.h>

/* ===== ARQUITECTURA ===== */
#include "portable/timers_arch_list.h"
#define TIMERS_ARCH             TIMERS_ARCH_CORTEX_M0P

/* ===== HEADERS ===== */
#define TIMERS_DEVICE_HEADER    "LPC845.h"
#define TIMERS_CORE_HEADER      "core_cm0plus.h"
/* ----------------------------------------------- */

// Cantidad maxima de timers
#define MAX_N_TIMERS            16

// Callback return type
#ifndef TIMER_CALLBACK_RET_TYPE
#define TIMER_CALLBACK_RET_TYPE uint8_t
#endif

// Usar callback context o parametros de tipo fijo
#define USE_CALLBACK_CONTEXT    1

// Argumentos de callback
#if USE_CALLBACK_CONTEXT
    #define TIMER_CALLBACK_ARGS void *context
#else
    #define TIMER_CALLBACK_ARGS void
#endif

// Usar queues o marcado de eventos
#define USE_QUEUES              1

// Largo de queue de timers (potencia de 2)
#define QUEUE_SIZE              16
#define QUEUE_MASK              (QUEUE_SIZE - 1)

// Cantidad de niveles de prioridad
#define N_TIMER_PRIORITIES      3

// Habilitacion de callbacks en ISR
#define USE_TIMER_CRITICAL      0
/* ----------------------------------------------- */

#ifndef USE_TIMER_CRITICAL
    #error "Port must define TIMERS_CRITICAL_ENTER"
#endif

#endif /* TIMERS_CONFIG_H_ */
