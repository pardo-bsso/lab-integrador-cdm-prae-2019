#ifndef _KBD_H
#define _KBD_H

#define NO_KEY      (0xFF)

#if defined(__cplusplus)
extern "C" {
#endif

void kbd_init_ports(void);

/*
 *  Scans the keyboard and returns the ascii code or NO_KEY.
 *
 *  Priority:
 *  1-9
 *  0
 *  A-D
 *  * #
 */
unsigned char scan_keyboard(void);

#if defined(__cplusplus)
}
#endif

#endif
