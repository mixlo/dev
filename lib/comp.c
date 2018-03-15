#include <stdlib.h>
#include <stdio.h>
#include <llist.h>
#include <comp.h>

struct comp {
    int time_sig_upper;
    int time_sig_lower;
    int bpm;
    llist_t *chords;
};

struct chord {
    int length;
    note_t *notes;
    int dur_denom;
};



// ##########  PRIVATE DECLARATIONS  ##########
static chord_t *chord_new(int length, note_t *notes, int dur_denom);
static void chord_free(void *chord);



// ##########  PUBLIC  ##########

comp_t *comp_new(int bpm)
{
    comp_t *new_comp = malloc(sizeof *new_comp);

    if (new_comp == NULL)
	return NULL;

    llist_t *chords = llist_new(&chord_free);
    
    if (chords == NULL)
	return NULL;

    new_comp->bpm = bpm;
    new_comp->chords = chords;

    // Currently only support for time signature 4/4
    new_comp->time_sig_upper = 4;
    new_comp->time_sig_lower = 4;
    
    return new_comp;
}

int comp_get_bpm(comp_t *comp)
{
    return comp->bpm;
}

void comp_add_chord(comp_t *comp, int length, note_t *notes, int dur_denom)
{
    chord_t *new_chord = chord_new(length, notes, dur_denom);
    llist_append(comp->chords, new_chord);
}

void comp_add_chord_1(comp_t *comp, note_t n1, int dur_denom)
{
    note_t *notes = malloc(sizeof *notes * 1);
    notes[0] = n1;
    comp_add_chord(comp, 1, notes, dur_denom);
}

void comp_add_chord_2(comp_t *comp, note_t n1, note_t n2, int dur_denom)
{
    note_t *notes = malloc(sizeof *notes * 2);
    notes[0] = n1;
    notes[1] = n2;
    comp_add_chord(comp, 2, notes, dur_denom);
}

void comp_add_chord_3(comp_t *comp, note_t n1, note_t n2, note_t n3, int dur_denom)
{
    note_t *notes = malloc(sizeof *notes * 3);
    notes[0] = n1;
    notes[1] = n2;
    notes[2] = n3;
    comp_add_chord(comp, 3, notes, dur_denom);
}

void comp_add_chord_4(comp_t *comp, note_t n1, note_t n2, note_t n3, note_t n4, int dur_denom)
{
    note_t *notes = malloc(sizeof *notes * 4);
    notes[0] = n1;
    notes[1] = n2;
    notes[2] = n3;
    notes[3] = n4;
    comp_add_chord(comp, 4, notes, dur_denom);
}

int comp_get_chord_size(chord_t *chord)
{
    return chord->length;
}

note_t *comp_get_chord_notes(chord_t *chord)
{
    return chord->notes;
}

// Todo: implement dotted notes by geometric series
double comp_get_chord_duration(comp_t *comp, chord_t *chord)
{
    return 60.0 / (comp->bpm * chord->dur_denom) * comp->time_sig_lower;
}

llist_iter_t *comp_get_chords_iterator(comp_t *comp)
{
    return llist_get_iter(comp->chords);
}

void comp_free(comp_t *comp)
{
    llist_free(comp->chords);
    free(comp);
}



// ##########   PRIVATE   ##########

static chord_t *chord_new(int length, note_t *notes, int dur_denom)
{
    chord_t *chord = malloc(sizeof *chord);

    if (chord != NULL) {
	chord->length = length;
	chord->notes = notes;
	chord->dur_denom = dur_denom;
    }
    
    return chord;
}

static void chord_free(void *chord)
{
    chord_t *c = chord;
    free(c->notes);
    free(c);
}
