#ifndef TIMERS_PORT_SELECT_H
#define TIMERS_PORT_SELECT_H

#include "timers_config.h"

#if (TIMERS_CORE == TIMERS_CORE_AVR)
    #include "port/avr.h"

#elif (TIMERS_CORE == TIMERS_CORE_PIC16)
    #include "port/pic16.h"

#elif (TIMERS_CORE == TIMERS_CORE_PIC18)
    #include "port/pic18.h"

#elif (TIMERS_CORE == TIMERS_CORE_MSP430)
    #include "port/msp430.h"

#elif (TIMERS_CORE == TIMERS_CORE_COLDFIRE_V2)
    #include "port/coldfire_v2.h"

#elif (TIMERS_CORE == TIMERS_CORE_AVR32)
    #include "port/avr32.h"

#elif (TIMERS_CORE == TIMERS_CORE_CORTEX_M0)
    #include "port/cortex_m0.h"

#elif (TIMERS_CORE == TIMERS_CORE_CORTEX_M3)
    #include "port/cortex_m3.h"

#elif (TIMERS_CORE == TIMERS_CORE_CORTEX_M4)
    #include "port/cortex_m4.h"

#elif (TIMERS_CORE == TIMERS_CORE_CORTEX_M33)
    #include "port/cortex_m33.h"

#elif (TIMERS_CORE == TIMERS_CORE_RISCV)
    #include "port/riscv.h"

#else
    #error "TIMERS_CORE not defined or unsupported"
#endif

#endif