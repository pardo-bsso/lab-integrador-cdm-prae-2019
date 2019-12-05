#ifndef _MUSIC_H
#define _MUSIC_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "notes.h"

#define SPEED_NORMAL    (0x80)
#define SPEED_HALF      (0xFF)
#define SPEED_DOUBLE    (0x00)

#define NO_CURRENT_NOTE ((signed int)-1)

enum PLAY_STATE {
    STOPPED,
    PAUSED,
    PLAYING,
};

enum PLAY_MODE {
    NORMAL = 0,
    REVERSE = 1,
    LOOP = 2,
};

struct note_info {
    unsigned long note_number;
    unsigned long duration;
};

struct song {
    unsigned long length;
    struct note_info *notes;
};

unsigned int scale_note_duration(unsigned int duration, unsigned char scale);

void music_init_ports(void);
void music_set_song(struct song *target_song);
void music_set_mode(enum PLAY_MODE mode);
void music_play(void);
void music_stop(void);
unsigned char music_is_playing(void);
void music_tick(void);


#define DEFAULT_SONG    ((struct song *)0)


#if defined(__cplusplus)
}
#endif

#endif
