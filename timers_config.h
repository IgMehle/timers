/*
 * timers_config.h
 *
 *  Created on: 25 apr. 2026
 *      Author: Ignacio Mehle
 */

#ifndef TIMERS_CONFIG_H_
#define TIMERS_CONFIG_H_

// Cantidad maxima de timers
#define MAX_N_TIMERS            16U

// Usar queues o marcado de eventos
#define USE_QUEUES              1U

// Largo de queue de timers (potencia de 2)
#define QUEUE_SIZE              64U
#define QUEUE_MASK              (QUEUE_SIZE - 1)

// Cantidad de niveles de prioridad
#define N_PRIORITIES            3U

// Prioridad maxima de soft timer (dentro de ISR)
#define TIMER_CRITICAL          0xFF

// Habilitacion de callbacks en ISR
#define TIMER_CRITICAL_ENABLED  0U

/* ----------------------------------------------- */
/* ===== CORES ===== */
#define TIMERS_CORE_AVR         1
#define TIMERS_CORE_PIC16       2
#define TIMERS_CORE_PIC18       3
#define TIMERS_CORE_MSP430      4
#define TIMERS_CORE_COLDFIRE_V2 5
#define TIMERS_CORE_AVR32       6
#define TIMERS_CORE_CORTEX_M0   7
#define TIMERS_CORE_CORTEX_M3   8
#define TIMERS_CORE_CORTEX_M4   9
#define TIMERS_CORE_CORTEX_M33  10
#define TIMERS_CORE_RISCV       11

/* ===== ARQUITECTURA ===== */
#define TIMERS_CORE             TIMERS_CORE_CORTEX_M0

/* ----------------------------------------------- */
/* ===== TAGS DE PRIORIDADES ===== */
#if N_PRIORITIES == 2
    #define TIMER_HIGH_PRIORITY 0U
    #define TIMER_LOW_PRIORITY  1U
#endif

#if N_PRIORITIES == 3
    #define TIMER_HIGH_PRIORITY 0U
    #define TIMER_MID_PRIORITY  1U
    #define TIMER_LOW_PRIORITY  2U
#endif
/* ----------------------------------------------- */

#ifndef TIMER_CRITICAL_ENABLED
#error "Port must define TIMERS_CRITICAL_ENTER"
#endif

#endif /* TIMERS_H_ */
