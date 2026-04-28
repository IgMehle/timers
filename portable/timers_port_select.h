/**
 * timers_port_select.h
 *
 *  Created on: 26 apr. 2026
 *      Author: Ignacio Mehle
 */
#ifndef TIMERS_PORT_SELECT_H_
#define TIMERS_PORT_SELECT_H_

#include <timers_config.h>

#if (TIMERS_ARCH == TIMERS_ARCH_AVR)
    #include "avr.h"

#elif (TIMERS_ARCH == TIMERS_ARCH_PIC16)
    #include "pic16.h"

#elif (TIMERS_ARCH == TIMERS_ARCH_PIC18)
    #include "pic18.h"

#elif (TIMERS_ARCH == TIMERS_ARCH_MSP430)
    #include "msp430.h"

#elif (TIMERS_ARCH == TIMERS_ARCH_COLDFIRE_V2)
    #include "coldfire_v2.h"

#elif (TIMERS_ARCH == TIMERS_ARCH_AVR32)
    #include "avr32.h"

#elif (TIMERS_ARCH == TIMERS_ARCH_CORTEX_M0)
    #include "arm_cm0.h"

#elif (TIMERS_ARCH == TIMERS_ARCH_CORTEX_M0P)
    #include "arm_cm0plus.h"

#elif (TIMERS_ARCH == TIMERS_ARCH_CORTEX_M3)
    #include "arm_cm3.h"

#elif (TIMERS_ARCH == TIMERS_ARCH_CORTEX_M4)
    #include "arm_cm4.h"

#elif (TIMERS_ARCH == TIMERS_ARCH_CORTEX_M33)
    #include "arm_cm33.h"

#elif (TIMERS_ARCH == TIMERS_ARCH_RISC_V)
    #include "risc_v.h"

#else
    #error "TIMERS_ARCH not defined or unsupported"
#endif

#endif /* TIMERS_PORT_SELECT_H_ */