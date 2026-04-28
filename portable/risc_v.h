/**
 * risc_v.h
 *
 *  Created on: 26 apr. 2026
 *      Author: Ignacio Mehle
 */
#ifndef TIMERS_PORTABLE_RISC_V_H_
#define TIMERS_PORTABLE_RISC_V_H_
 
#include "../timers_config.h"

/* ====== DEVICE HEADER ====== */
#include DEVICE_HEADER

/* ====== CORE HEADER ====== */
#include TIMERS_CORE_HEADER

/* ====== SECCION CRITICA ====== */
#define TIMERS_CRITICAL_ENTER()     __asm volatile("csrci mstatus, 8")
#define TIMERS_CRITICAL_EXIT()      __asm volatile("csrsi mstatus, 8")

/* ====== DEFERRED ISR ====== */
#define TIMERS_USE_DEFERRED_ISR     (0)

/* ====== OPTIMIZACIONES ====== */
// si es RV32
#define TIMERS_ATOMIC_32            (1)

#endif /* TIMERS_PORTABLE_RISC_V_H_ */