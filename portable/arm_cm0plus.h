/**
 * arm_cm0plus.h
 *
 *  Created on: 26 apr. 2026
 *      Author: Ignacio Mehle
 */
#ifndef TIMERS_PORTABLE_ARM_CM0PLUS_H_
#define TIMERS_PORTABLE_ARM_CM0PLUS_H_

#include "../timers_config.h"

/* ====== DEVICE HEADER ====== */
#include TIMERS_DEVICE_HEADER

/* ====== CORE HEADER ====== */
// #include TIMERS_CORE_HEADER
// #include <core_cm0plus.h>

/* ====== SECCION CRITICA ====== */
#define TIMERS_CRITICAL_ENTER()         __disable_irq()
#define TIMERS_CRITICAL_EXIT()          __enable_irq()

/* ====== DEFERRED ISR ====== */
#define TIMERS_USE_DEFERRED_ISR         (1)

#if TIMERS_USE_DEFERRED_ISR
    #define TIMERS_TRIGGER_DEFERRED()   (SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk)
#endif

/* ====== OPTIMIZACIONES ====== */
#define TIMERS_ATOMIC_32                (1)

#endif /* TIMERS_PORTABLE_ARM_CM0PLUS_H_ */