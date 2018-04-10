#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <llist.h>
#include <comp.h>

struct comp {
    int time_sig_upper;
    int time_sig_lower;
    int bpm;
    llist_t *trbl_chords;
    llist_t *bass_chords;
};

struct chord {
    int length;
    note_t *notes;
    int dur_denom;
    bool dotted;
};



// ##########  PRIVATE DECLARATIONS  ##########
static chord_t *chord_new(int length, note_t *notes, int dur_denom, bool dotted);
static void chord_free(void *chord);



// ##########  PUBLIC  ##########

comp_t *comp_new(int bpm)
{
    comp_t *new_comp = malloc(sizeof *new_comp);

    if (new_comp == NULL)
	return NULL;

    llist_t *trbl_chords = llist_new(&chord_free);
    llist_t *bass_chords = llist_new(&chord_free);
    
    if (trbl_chords == NULL || bass_chords == NULL) {
	free(trbl_chords);
	free(bass_chords);
	return NULL;
    }

    new_comp->bpm = bpm;
    new_comp->trbl_chords = trbl_chords;
    new_comp->bass_chords = bass_chords;

    // Currently only support for time signature 4/4
    new_comp->time_sig_upper = 4;
    new_comp->time_sig_lower = 4;
    
    return new_comp;
}

int comp_get_bpm(comp_t *comp)
{
    return comp->bpm;
}

void comp_add_chord(
    comp_t *comp,
    int length,
    clef_t clef,
    note_t *notes,
    int dur_denom,
    bool dotted)
{
    chord_t *new_chord = chord_new(length, notes, dur_denom, dotted);
    llist_append(clef == TRBL ? comp->trbl_chords : comp->bass_chords, new_chord);
}

void comp_add_chord_1(
    comp_t *comp,
    clef_t clef,
    note_t n1,
    int dur_denom,
    bool dotted)
{
    note_t *notes = malloc(sizeof *notes * 1);
    notes[0] = n1;
    comp_add_chord(comp, 1, clef, notes, dur_denom, dotted);
}

void comp_add_chord_2(
    comp_t *comp,
    clef_t clef,
    note_t n1,
    note_t n2,
    int dur_denom,
    bool dotted)
{
    note_t *notes = malloc(sizeof *notes * 2);
    notes[0] = n1;
    notes[1] = n2;
    comp_add_chord(comp, 2, clef, notes, dur_denom, dotted);
}

void comp_add_chord_3(
    comp_t *comp,
    clef_t clef,
    note_t n1,
    note_t n2,
    note_t n3,
    int dur_denom,
    bool dotted)
{
    note_t *notes = malloc(sizeof *notes * 3);
    notes[0] = n1;
    notes[1] = n2;
    notes[2] = n3;
    comp_add_chord(comp, 3, clef, notes, dur_denom, dotted);
}

void comp_add_chord_4(
    comp_t *comp,
    clef_t clef,
    note_t n1,
    note_t n2,
    note_t n3,
    note_t n4,
    int dur_denom,
    bool dotted)
{
    note_t *notes = malloc(sizeof *notes * 4);
    notes[0] = n1;
    notes[1] = n2;
    notes[2] = n3;
    notes[3] = n4;
    comp_add_chord(comp, 4, clef, notes, dur_denom, dotted);
}

void comp_add_rest(
    comp_t *comp,
    clef_t clef,
    int dur_denom)
{
    note_t *rest = malloc(sizeof *rest);
    *rest = REST;
    comp_add_chord(comp, 1, clef, rest, dur_denom, false);
}

int comp_get_chord_size(chord_t *chord)
{
    return chord->length;
}

note_t *comp_get_chord_notes(chord_t *chord)
{
    return chord->notes;
}

double comp_get_chord_duration(comp_t *comp, chord_t *chord)
{
    double dot_mult = chord->dotted ? 2.0 / 3.0 : 1;
    return 60.0 / (comp->bpm * chord->dur_denom * dot_mult) * comp->time_sig_lower;
}

llist_iter_t *comp_get_chords_iterator(comp_t *comp, clef_t clef)
{
    return llist_get_iter(clef == TRBL ? comp->trbl_chords : comp->bass_chords);
}

void comp_free(comp_t *comp)
{
    llist_free(comp->trbl_chords);
    llist_free(comp->bass_chords);
    free(comp);
}



// ##########   PRIVATE   ##########

static chord_t *chord_new(int length, note_t *notes, int dur_denom, bool dotted)
{
    chord_t *chord = malloc(sizeof *chord);

    if (chord != NULL) {
	chord->length = length;
	chord->notes = notes;
	chord->dur_denom = dur_denom;
	chord->dotted = dotted;
    }
    
    return chord;
}

static void chord_free(void *chord)
{
    chord_t *c = chord;
    free(c->notes);
    free(c);
}
