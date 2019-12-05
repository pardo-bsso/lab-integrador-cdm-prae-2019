#include "kbd.h"

#if defined(__AVR__)
#include <Arduino.h>
#include <HardwareSerial.h>
#else
#include <MC9S08JM32.h>
#endif


#if defined(__AVR__)
void kbd_init_ports(void)
{
    Serial.begin(9600);
}

#else
void kbd_init_ports(void)
{
    PTBDD = 0xF0;
    PTBPE = 0xFF;
    PTBD = 0x00;

}
#endif



/*
 *  PTB
 *
 *  7   6   5   4       3   2   1   0
 *  filas teclado       entrada columnas teclado
 *  F1  F2  F3  F4      C1  C2  C3  C4
 *
 *
 *      1   2   3   A
 *      4   5   6   B
 *      7   8   9   C
 *      *   0   #   D
 *
 */

#if defined(__AVR__)
unsigned char scan_keyboard(void)
{
    if (Serial.available()) {
        return Serial.read();
    } else {
        return NO_KEY;
    }
}

#else

/*
 * Sets the corresponding row line to 0
 */

void set_row(unsigned char row)
{
    unsigned char row_bit = 0x80 >> row;
    unsigned char row_pattern = (~row_bit) & 0xF0;

    PTBD = row | (PTBD & 0x0F);
}


/*
 * Tests if the column has a pressed key
 */
unsigned char column_has_pressed_key(unsigned char column)
{
    unsigned char col_bit = 0x08 >> column;
    unsigned char col_pattern = (~col_bit) & 0x0F;

    return !(PTBD & col_bit);
}


/*
 *  Scans the keyboard and returns the ascii code or NO_KEY.
 *
 *  Priority:
 *  1-9
 *  0
 *  A-D
 *  * #
 */
unsigned char raw_scan_keyboard(void)
{
    unsigned char row_idx = 0;
    unsigned char col_idx = 0;

    // 1 - 9
    for (row_idx = 0; row_idx < 3; row_idx++) {
        set_row(row_idx);
        for (col_idx = 0; col_idx < 3; col_idx++) {
            if (column_has_pressed_key(col_idx)) {
                return ('1' + 3*row_idx + col_idx);
            }
        }
    }

    // 0
    set_row(3);
    if (column_has_pressed_key(1)) {
        return '0';
    }

    // A-D
    for (row_idx = 0; row_idx < 4; row_idx++) {
        set_row(row_idx);

        if (column_has_pressed_key(3)) {
            return ('A' + row_idx);
        }
    }

    // * #
    set_row(3);

    if (column_has_pressed_key(0)) {
        return '*';
    }

    if (column_has_pressed_key(2)) {
        return '#';
    }

    return NO_KEY;
}

unsigned char scan_keyboard(void)
{
    static char last_detected_key = NO_KEY;

    char current_key = raw_scan_keyboard();

    if ((current_key != NO_KEY) && (last_detected_key != NO_KEY)) {
        return NO_KEY;
    }

    if (last_detected_key == NO_KEY) {
        last_detected_key = current_key;
        return current_key;
    }

    return NO_KEY;
}
#endif
