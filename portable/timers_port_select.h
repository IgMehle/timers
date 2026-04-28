#ifndef TIMERS_PORT_SELECT_H
#define TIMERS_PORT_SELECT_H

#include "timers_config.h"

#if (TIMERS_CORE == TIMERS_CORE_AVR)
    #include "avr.h"

#elif (TIMERS_CORE == TIMERS_CORE_PIC16)
    #include "pic16.h"

#elif (TIMERS_CORE == TIMERS_CORE_PIC18)
    #include "pic18.h"

#elif (TIMERS_CORE == TIMERS_CORE_MSP430)
    #include "msp430.h"

#elif (TIMERS_CORE == TIMERS_CORE_COLDFIRE_V2)
    #include "coldfire_v2.h"

#elif (TIMERS_CORE == TIMERS_CORE_AVR32)
    #include "avr32.h"

#elif (TIMERS_CORE == TIMERS_CORE_CORTEX_M0)
    #include "arm_cm0.h"

#elif (TIMERS_CORE == TIMERS_CORE_CORTEX_M0P)
    #include "arm_cm0plus.h"

#elif (TIMERS_CORE == TIMERS_CORE_CORTEX_M3)
    #include "cortex_m3.h"

#elif (TIMERS_CORE == TIMERS_CORE_CORTEX_M4)
    #include "cortex_m4.h"

#elif (TIMERS_CORE == TIMERS_CORE_CORTEX_M33)
    #include "cortex_m33.h"

#elif (TIMERS_CORE == TIMERS_CORE_RISCV)
    #include "riscv.h"

#else
    #error "TIMERS_CORE not defined or unsupported"
#endif

#endif