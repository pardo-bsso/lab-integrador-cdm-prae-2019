/*
 * RW fijo en 0
 * HCS08:   data    PTE0-3
 *          RS      PTE4
 *          RE      PTE5
 *
 * Arduino:
 *          data    D2-D5
 *          RS      D12
 *          RE      D11
 */
#ifndef _LCD_PINS_H
#define _LCD_PINS_H

#if defined(__AVR__)
    #include <avr/io.h>
    #define LCD_RS              (PORTB4)
    #define LCD_RE              (PORTB3)
    // PORTD2 - PORTD5
    #define LCD_DATA            (PORTD)
#else

    #include <MC9S08JM32.h>
    #define LCD_RS              (PTED_PTED4)
    #define LCD_RS_CONTROL      (PTEDD_PTEDD4)

    #define LCD_RE              (PTED_PTED5)
    #define LCD_RE_CONTROL      (PTEDD_PTEDD5)

    #define LCD_DATA            (PTED)
    #define LCD_DATA_CONTROL    (PTEDD)
#endif

#endif
