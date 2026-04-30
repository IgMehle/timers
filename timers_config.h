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
#define MAX_N_TIMERS            (255)

/**
 * @brief Tipo del valor de retorno de los callbacks.
 * Puede redefinirse antes de incluir este header.
 */
#ifndef TIMER_CALLBACK_RET_TYPE
#define TIMER_CALLBACK_RET_TYPE uint8_t
#endif

/**
 * @brief Habilita el campo `context` en timer_t y su pasaje automatico al callback.
 *
 * USE_CALLBACK_CONTEXT = 1:
 *   - timer_t incluye un campo `void *context`
 *   - Los callbacks reciben ese puntero como argumento
 *   - Permite pasarle estado arbitrario al callback sin variables globales
 *
 * USE_CALLBACK_CONTEXT = 0:
 *   - Los callbacks no reciben argumentos
 *   - Util si todos los callbacks acceden al estado por otros medios (globales, etc.)
 */
#define USE_CALLBACK_CONTEXT    (1)

/**
 * @brief Firma de los argumentos del callback segun la configuracion.
 *
 * El nombre `context` en el typedef es ignorado por el compilador pero
 * sirve como documentacion: indica que ese void* es el estado del llamador,
 * no un argumento de proposito general.
 *
 * Consistente con el campo `void *context` de timer_t y con la convencion
 * de librerias como POSIX (pthread), FreeRTOS (pvParameters) y GTK (user_data).
 */
#if USE_CALLBACK_CONTEXT
    #define TIMER_CALLBACK_ARGS void *context
#else
    #define TIMER_CALLBACK_ARGS void
#endif

// Usar queues o marcado de eventos
#define USE_QUEUES              (1)

// Largo de queue de timers (potencia de 2)
#define TIMERS_QUEUE_SIZE       (16)
#define TIMERS_QUEUE_MASK       (TIMERS_QUEUE_SIZE - 1)

// Cantidad de niveles de prioridad
#define N_TIMER_PRIORITIES      (3)

// Habilitacion de callbacks en ISR
#define USE_TIMER_CRITICAL      (0)

// Prescaler de timers
#define TIMERS_PRESCALER_VALUE   (1000)

// Campos reload/ticks de 16/32 bits
#define USE_16BIT_COUNTERS      (1)
/* ----------------------------------------------- */

#ifndef USE_TIMER_CRITICAL
    #error "Port must define TIMERS_CRITICAL_ENTER"
#endif

#endif /* TIMERS_CONFIG_H_ */
