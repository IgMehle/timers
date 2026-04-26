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

// CORTEX M
// Habilitacion de PendSV Handler
#define PENDSV_ENABLED          0U
#if PENDSV_ENABLED == 1
#define PENDSV_TRIGGER()  SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk
#endif

//----------------------------------------//
//----- labels: niveles de prioridad -----//
//----------------------------------------//
#if N_PRIORITIES == 2
#define TIMER_HIGH_PRIORITY 0U
#define TIMER_LOW_PRIORITY  1U
#endif

#if N_PRIORITIES == 3
#define TIMER_HIGH_PRIORITY 0U
#define TIMER_MID_PRIORITY  1U
#define TIMER_LOW_PRIORITY  2U
#endif
//---------------------------------------------//

#endif /* TIMERS_H_ */
