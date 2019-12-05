#include "timeutils.h"

#if defined(__AVR__)
#include <Arduino.h>
#else
//HCS08
#include <MC9S08JM32.h>

volatile unsigned long __now = 0;

void delay(unsigned int millis)
{
    unsigned long start = __now;
    unsigned long duration = millis  * 32;

    while((__now - start) <= duration){
        __RESET_WATCHDOG();
    }
}

void tick(void)
{
    __now++;
    RTCSC |= RTCSC_RTIF_MASK;
}
#endif

// Tiempo en ticks de 31.25us / 32us
unsigned long get_current_time(void)
{
#if defined(__AVR__)
    return micros() / 32;
#else
    return __now;
#endif
}
