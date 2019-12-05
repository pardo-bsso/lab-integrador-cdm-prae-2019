#if defined(__AVR__)
#include "Arduino.h"
#else
#include <MC9S08JM32.h>
#endif


#include "timeutils.h"
#include "music.h"

#if defined(__AVR__)
void music_init_ports(void)
{
    pinMode(A0, OUTPUT);
    digitalWrite(A0, 0);
}

void toggle_speaker(void)
{
    digitalWrite(A0, !digitalRead(A0));
}

void turn_off_speaker(void)
{
    digitalWrite(A0, 0);
}
#else

void music_init_ports(void)
{
    PTADD = 0xFF;
    PTAD = 0;
}

void toggle_speaker(void)
{
    PTAD = ~PTAD;
}

void turn_off_speaker(void)
{
    PTAD = 0;
}
#endif

struct __play_state {
    signed   int current_note_idx;
    unsigned long note_start_time;
    unsigned long note_last_toggle_time;
    unsigned long note_duration;
    unsigned long note_modulo;
    unsigned char speed;
    enum PLAY_MODE mode;
    enum PLAY_STATE state;
    struct song *song;
} play_state = {
    NO_CURRENT_NOTE,
    0,
    0,
    0,
    0,
    SPEED_NORMAL,
    NORMAL,
    STOPPED,
    DEFAULT_SONG,
};


void music_set_mode(enum PLAY_MODE mode)
{
    play_state.mode = mode;
}

void music_play(void)
{
    struct song *song = play_state.song;

    music_stop();

    if (song) {
        play_state.state = PLAYING;
    } else {
    }
}

void music_stop(void)
{
    play_state.state = STOPPED;
    play_state.current_note_idx = NO_CURRENT_NOTE;
    turn_off_speaker();
}

unsigned char music_is_playing(void)
{
    return (play_state.state == PLAYING);
}

void music_set_song(struct song *target_song)
{
    music_stop();

    if (target_song) {
        play_state.song = target_song;
    }
}

void music_tick(void)
{
    unsigned long now = get_current_time();
    struct song *song = play_state.song;
    signed int note_idx;

    if (!song) {
        return;
    }

    if (play_state.state != PLAYING) {
        return;
    }

    if (play_state.current_note_idx == NO_CURRENT_NOTE) {
        play_state.note_start_time = now;
        play_state.note_duration = 0;
    }

    if ((now - play_state.note_start_time) >= play_state.note_duration) {
        play_state.current_note_idx++;

        if (play_state.current_note_idx >= song->length) {

            if (play_state.mode & LOOP) {
                play_state.current_note_idx = 0;
            } else {
                music_stop();
                return;
            }
        }

        note_idx = play_state.current_note_idx;

        if (play_state.mode & REVERSE) {
            note_idx = song->length - note_idx - 1;
        }

        play_state.note_start_time = now;
        play_state.note_last_toggle_time = now;
        play_state.note_duration = scale_note_duration(song->notes[note_idx].duration, play_state.speed);
        play_state.note_duration = song->notes[note_idx].duration;
        play_state.note_modulo = song->notes[note_idx].note_number;
    }


    if (!play_state.note_modulo) {
        turn_off_speaker();
    } else {
        if ((now - play_state.note_last_toggle_time) >= play_state.note_modulo) {
            toggle_speaker();
            play_state.note_last_toggle_time = now;
        }
    }
}

unsigned int scale_note_duration(unsigned int duration, unsigned char scale)
{
    unsigned long tmp;

    if (scale == 0x80) {
        return duration;
    }

    if (scale > 0x80) {
        tmp = ((unsigned long)duration * (scale - 0x80)) / 0x80;
        return duration + (unsigned int)tmp;
    } else {
        tmp = ((unsigned long)duration * (0x80 - scale)) / 0xFF;
        return duration - (unsigned int)tmp;
    }
}
