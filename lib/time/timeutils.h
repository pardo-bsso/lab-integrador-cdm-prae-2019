#ifndef _TIMEUTILS_H
#define _TIMEUTILS_H

#ifdef __cplusplus
 extern "C" {
#endif

#if defined(__AVR__)
#else
//HCS08
void tick(void);
void delay(unsigned int millis);

#endif

unsigned long get_current_time(void);


#ifdef __cplusplus
}
#endif

#endif
