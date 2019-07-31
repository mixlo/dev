#ifndef __COMP_H__
#define __COMP_H__

#include <llist.h>

// Chord options
#define NONE   0x0
#define DOTTED 0x1
#define BEND   0x2

typedef enum clef clef_t;
typedef enum note note_t;
typedef struct comp comp_t;
typedef struct chord chord_t;

enum clef {
    TRBL, BASS
};

enum note {
    C3, CS3, D3, DS3, E3, F3, FS3, G3, GS3, A3, AS3, B3,
    C4, CS4, D4, DS4, E4, F4, FS4, G4, GS4, A4, AS4, B4,
    C5, CS5, D5, DS5, E5, F5, FS5, G5, GS5, A5, AS5, B5,
    C6, CS6, D6, DS6, E6, F6, FS6, G6, GS6, A6, AS6, B6,
    REST
};

comp_t *comp_new(int bpm);
int comp_get_bpm(comp_t *comp);
void comp_add_chord(
    comp_t *comp,
    int length,
    clef_t clef,
    note_t *notes,
    int dur_denom,
    int options);
void comp_add_chord_1(
    comp_t *comp,
    clef_t clef,
    note_t n1,
    int dur_denom,
    int options);
void comp_add_chord_2(
    comp_t *comp,
    clef_t clef,
    note_t n1,
    note_t n2,
    int dur_denom,
    int options);
void comp_add_chord_3(
    comp_t *comp,
    clef_t clef,
    note_t n1,
    note_t n2,
    note_t n3,
    int dur_denom,
    int options);
void comp_add_chord_4(
    comp_t *comp,
    clef_t clef,
    note_t n1,
    note_t n2,
    note_t n3,
    note_t n4,
    int dur_denom,
    int options);
void comp_add_rest(
    comp_t *comp,
    clef_t clef,
    int dur_denom,
    int options);
int comp_get_chord_size(chord_t *chord);
note_t *comp_get_chord_notes(chord_t *chord);
double comp_get_chord_duration(comp_t *comp, chord_t *chord);
llist_iter_t *comp_get_chords_iterator(comp_t *comp, clef_t clef);
void comp_free(comp_t *comp);

#endif
