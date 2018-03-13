#ifndef __COMP_H__
#define __COMP_H__

typedef enum note note_t;
typedef struct chord chord_t;
typedef struct comp comp_t;

comp_t *comp_new(int bpm);
void comp_add_chord(comp_t *comp, note_t *notes);
void comp_add_chord_1(comp_t *comp, note_t n1);
void comp_add_chord_2(comp_t *comp, note_t n1, note_t n2);
void comp_add_chord_3(comp_t *comp, note_t n1, note_t n2, note_t n3);
void comp_add_chord_4(comp_t *comp, note_t n1, note_t n2, note_t n3, note_t n4);

#endif
