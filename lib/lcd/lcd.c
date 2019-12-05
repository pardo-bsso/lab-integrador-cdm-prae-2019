#include "timeutils.h"
#include "lcd.h"
#include "lcd_pins.h"
#include "lcd_commands.h"

unsigned char flip_nibble(unsigned char nibble);

#if defined(__AVR__)
#include <Arduino.h>
// AVR
void lcd_init_ports(void)
{
    DDRD |= 0B11111100;

    DDRB |= 0B00011000;
}

void lcd_set_rs(unsigned char rs)
{
    if (rs) {
        PORTB |= _BV(LCD_RS);
    } else {
        PORTB &= ~_BV(LCD_RS);
    }
}

void lcd_set_re(unsigned char re)
{
    if (re) {
        PORTB |= _BV(LCD_RE);
    } else {
        PORTB &= ~_BV(LCD_RE);
    }
}
#else
// HCS08

void lcd_init_ports(void)
{
    LCD_RS_CONTROL = 1;
    LCD_RE_CONTROL = 1;
    LCD_DATA_CONTROL = 0xFF;
}

void lcd_set_rs(unsigned char rs)
{
    if (rs) {
        LCD_RS = 1;
    } else {
        LCD_RS = 0;
    }
}

void lcd_set_re(unsigned char re)
{
    if (re) {
        LCD_RE = 1;
    } else {
        LCD_RE = 0;
    }
}
// HCS08
#endif


unsigned char flip_nibble(unsigned char nibble)
{
    return 0x0F & (((nibble & 0x08) >> 3) | ((nibble & 0x01) << 3) | ((nibble & 0x02) << 1) | ((nibble & 0x04) >> 1));
}

void lcd_send_data(unsigned char data)
{
    unsigned char port_data;
    unsigned char low, high;

    port_data = 0;
    low = 0;
    high = 0;

    // para matchear pinout por defecto con la libreria LiquidCrystal
    low =  flip_nibble(data & 0x0F);
    high = flip_nibble((data & 0xF0) >> 4);

    lcd_set_re(1);
    lcd_delay();

#if defined(__AVR__)
    // Pines 0 y 1 asignados al puerto serie.
    port_data = (LCD_DATA & 0b11) | (high << 2);
#else
    port_data = (LCD_DATA & 0xF0) | (high);
#endif
    LCD_DATA = port_data;

    lcd_delay();
    lcd_set_re(0);
    lcd_delay();

    lcd_set_re(1);
    lcd_delay();

#if defined(__AVR__)
    // Pines 0 y 1 asignados al puerto serie.
    port_data = (LCD_DATA & 0b11) | (low << 2);
#else
    port_data = (LCD_DATA & 0xF0) | (low);
#endif
    LCD_DATA = port_data;

    lcd_delay();
    lcd_set_re(0);
    lcd_delay();
}

void lcd_command(unsigned char command)
{
    lcd_set_rs(0);
    lcd_send_data(command);
}

void lcd_reset(void)
{
    lcd_set_rs(0);
    lcd_set_re(0);
    delay(100);

    // modo 4 bit, 2 lineas, caracteres 5x8
    // Ver datasheet HD44780U seccion "Initializing by Instruction"

    lcd_command(0b00110011);
    lcd_command(0b00110010);

    //2 lineas 5x8
    lcd_command(0b00101000);

    lcd_command(0b00001000);
    lcd_command(0b00000001);
    lcd_command(0b00000110);
    lcd_command(0b00001100);

}

void lcd_delay(void)
{
    delay(5);
}

void lcd_putchar(char c)
{
    lcd_set_rs(1);
    lcd_send_data(c);
}


void lcd_printf(char const *c)
{
    unsigned int idx = 0;

    while(c[idx]) {
        lcd_putchar(c[idx]);
        idx++;
    }
}


void lcd_clear(void)
{
    lcd_command(0x01);
}


void lcd_goto(unsigned int line, unsigned int col)
{
    unsigned int address = col;

    if (line) {
        address += line * 40;
    }

    lcd_command(0x80 | (address & 0x3F));
}
