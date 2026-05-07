/**
 * arm_cm0.h
 *
 *  Created on: 26 apr. 2026
 *      Author: Ignacio Mehle
 */
#ifndef TIMERS_PORTABLE_ARM_CM0_H_
#define TIMERS_PORTABLE_ARM_CM0_H_

#include "../timers_config.h"

/* ====== DEVICE HEADER ====== */
#include TIMERS_DEVICE_HEADER

/* ====== CORE HEADER ====== */
// #include TIMERS_CORE_HEADER
#include <core_cm0.h>

/* ===== TIMER IRQ ===== */
#define TIMERS_DISABLE_TIMER_IRQ() \
    uint32_t _systick_state = SysTick->CTRL; \
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk
#define TIMERS_ENABLE_TIMER_IRQ() \
    SysTick->CTRL = _systick_state

/* ===== GENERAL IRQ ===== */
#define TIMERS_DISABLE_ALL_IRQ() \
    uint32_t _primask = __get_PRIMASK(); \
    __disable_irq()
#define TIMERS_ENABLE_ALL_IRQ()    \
    __set_PRIMASK(_primask)

/* ====== IRQ LOCK ====== */
#if TIMER_IRQ_ONLY_LOCK            
    #define TIMERS_IRQ_LOCK()           TIMERS_DISABLE_TIMER_IRQ()
    #define TIMERS_IRQ_UNLOCK()         TIMERS_ENABLE_TIMER_IRQ()
#else
    #define TIMERS_IRQ_LOCK()           TIMERS_DISABLE_ALL_IRQ()
    #define TIMERS_IRQ_UNLOCK()         TIMERS_ENABLE_ALL_IRQ()
#endif
// #define SYSTICK_IRQDIS	NVIC_DisableIRQ(SysTick_IRQn)
// #define SYSTICK_IRQEN	NVIC_EnableIRQ(SysTick_IRQn)

/* ====== ATOMICIDAD ====== */
#define TIMERS_ATOMIC_8                 (1)
#define TIMERS_ATOMIC_16                (0)
#define TIMERS_ATOMIC_32                (0)

/* ====== DEFERRED ISR ====== */
#define TIMERS_USE_DEFERRED_ISR         (1)

#if TIMERS_USE_DEFERRED_ISR
    #define TIMERS_TRIGGER_DEFERRED()   (SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk)
#endif

#endif /* TIMERS_PORTABLE_ARM_CM0_H_ */