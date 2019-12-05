#ifndef _LCD_H
#define _LCD_H

#if defined(__cplusplus)
extern "C" {
#endif


void lcd_init_ports(void);
void lcd_set_rs(unsigned char rs);
void lcd_set_re(unsigned char re);
void lcd_send_data(unsigned char data);
void lcd_command(unsigned char command);
void lcd_delay(void);
void lcd_putchar(char c);
void lcd_printf(char const *c);
void lcd_reset(void);
void lcd_clear(void);
void lcd_goto(unsigned int line, unsigned int col);

#if defined(__cplusplus)
}
#endif

#endif
