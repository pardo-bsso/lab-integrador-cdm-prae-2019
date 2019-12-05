#if defined(__AVR__)
#include <Arduino.h>
#include <HardwareSerial.h>
#else
#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "stdio.h"

extern "C" {
    void MCU_init(void); /* Device initialization function declaration */
}
#endif


#include "timeutils.h"
#include "lcd.h"
#include "kbd.h"
#include "music.h"

#include "password.h"
#include "songs.h"

// 500ms in 32us ticks
#define ALARM_PERIOD    (15625)
#define MAX_TRIES       (4)

#define TMP_BUF_LENGTH  (16)

void print_banner(void);
void start_alarm(void);
void alarm(void);
void open_lock(void);

unsigned long last_alarm_toggle;
unsigned int try_count = 0;
char buf[TMP_BUF_LENGTH];

enum LOCK_STATE {
    CLOSED,
    OPEN,
    ALARM,
} lock_state = CLOSED;




void setup(void)
{
#if defined(__AVR__)
    Serial.begin(9600);
    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
#else
    PTDDD = 0xFF;
    PTDDS = 0xFF;
    PTDD = 0x00;
#endif

    password_set_key("1234");

    kbd_init_ports();
    lcd_init_ports();
    music_init_ports();

    lcd_reset();
    music_set_song(&edvard_grieg_hall_of_mountain_king);

    print_banner();
}

void print_banner(void)
{
    lcd_clear();
    lcd_goto(0,0);

    lcd_printf("Ingrese clave:");
    lcd_goto(1,0);
}

void loop(void)
{
    unsigned char key = scan_keyboard();

    music_tick();

    if (lock_state == CLOSED && (key != NO_KEY)) {
        switch (password_process_key(key)) {
            case NONE:
                lcd_printf("*");
                break;

            case CORRECT_PASSWORD:
                lcd_clear();
                lcd_goto(1,0);
                lcd_printf("Acceso OK");
                open_lock();
                lock_state = OPEN;
                break;

            case WRONG_PASSWORD:
                try_count++;

                lcd_goto(0,0);
                lcd_printf("Error         ");

                lcd_goto(1,0);
#if defined(__AVR__)
                snprintf(buf, TMP_BUF_LENGTH, "%i intentos", MAX_TRIES - try_count);
#else
                (void)sprintf(buf, "%i intentos", MAX_TRIES - try_count);
#endif
                lcd_printf(buf);
                lcd_goto(1,0);

                if (try_count == MAX_TRIES) {
                    try_count = 0;
                    start_alarm();
                    lock_state = ALARM;
                } else {
                    delay(1000);
                    print_banner();
                }

                break;
        }
    }

    if (lock_state == ALARM) {
        alarm();
    }
}

void start_alarm(void)
{
    last_alarm_toggle = get_current_time();
    music_play();
}

void alarm(void)
{
    unsigned long now = get_current_time();

    if ((now - last_alarm_toggle) > ALARM_PERIOD) {
        last_alarm_toggle = now;
#if defined(__AVR__)
        digitalWrite(A1, !digitalRead(A1));
#else
    PTDD = (PTDD & ~0x02) | ~(PTDD & 0x02);
#endif
    }
}

void open_lock(void)
{
#if defined(__AVR__)
    digitalWrite(A2, 1);
#else
    PTDD = (PTDD & ~0x01) | 0x01;
#endif
}


#if defined(__AVR__)
#else
void main(void)
{

    MCU_init();

    setup();

    EnableInterrupts;
    for(;;) {
      __RESET_WATCHDOG();
      loop();
    }

}
#endif
