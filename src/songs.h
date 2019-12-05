#ifndef _SONGS_H
#define _SONGS_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "music.h"

static struct note_info edvard_grieg_hall_of_mountain_king_notes[] = {

        {D4, EIGTH},
        {E4, EIGTH},
        {F4, EIGTH},
        {G4, EIGTH},
        {A4, EIGTH},
        {F4, EIGTH},
        {A4, QUARTER},

        {G4_SHARP, EIGTH},
        {E4, EIGTH},
        {G4_SHARP, QUARTER},
        {G4, EIGTH},
        {D4_SHARP, EIGTH},
        {G4, QUARTER},


        {D4, EIGTH},
        {E4, EIGTH},
        {F4, EIGTH},
        {G4, EIGTH},
        {A4, EIGTH},
        {F4, EIGTH},
        {A4, EIGTH},
        {D5, EIGTH},

        {C5, EIGTH},
        {A4, EIGTH},
        {F4, EIGTH},
        {A4, EIGTH},
        {C5, HALF},

        {SILENCE, WHOLE},
};

static struct song edvard_grieg_hall_of_mountain_king = {
    sizeof(edvard_grieg_hall_of_mountain_king_notes) / sizeof(edvard_grieg_hall_of_mountain_king_notes[0]),
    edvard_grieg_hall_of_mountain_king_notes,
};


#if defined(__cplusplus)
}
#endif

#endif
