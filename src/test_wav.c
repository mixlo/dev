#include <stdio.h>
#include <comp.h>
#include <wav.h>

/*
void add_twinkle(comp_t *comp)
{
    comp_add_chord_2(comp, C5, C4, 4);
    comp_add_chord_2(comp, C5, C4, 4);
    comp_add_chord_2(comp, G5, E4, 4);
    comp_add_chord_2(comp, G5, E4, 4);
    comp_add_chord_2(comp, A5, F4, 4);
    comp_add_chord_2(comp, A5, F4, 4);
    comp_add_chord_2(comp, G5, E4, 2);
    comp_add_chord_2(comp, F5, D4, 4);
    comp_add_chord_2(comp, F5, D4, 4);
    comp_add_chord_2(comp, E5, C4, 4);
    comp_add_chord_2(comp, E5, C4, 4);
    comp_add_chord_2(comp, D5, G4, 4);
    comp_add_chord_2(comp, D5, G4, 4);
    comp_add_chord_2(comp, C5, C4, 2);
}

void add_something(comp_t *comp)
{
    comp_add_chord_2(comp, G5, C4, 4);
    comp_add_chord_2(comp, C5, C4, 8);
    comp_add_chord_2(comp, D5, C4, 8);
    comp_add_chord_2(comp, E5, C4, 8);
    comp_add_chord_2(comp, F5, C4, 8);
    comp_add_chord_2(comp, G5, C4, 4);
    comp_add_chord_2(comp, C5, C4, 4);
    comp_add_chord_2(comp, C5, E4, 4);

    comp_add_chord_2(comp, A5, F4, 4);
    comp_add_chord_2(comp, F5, F4, 8);
    comp_add_chord_2(comp, G5, F4, 8);
    comp_add_chord_2(comp, A5, F4, 8);
    comp_add_chord_2(comp, B5, F4, 8);
    comp_add_chord_2(comp, C6, C4, 4);
    comp_add_chord_2(comp, C5, C4, 4);
    comp_add_chord_2(comp, C5, E4, 4);

    comp_add_chord_2(comp, F5, F4, 4);
    comp_add_chord_2(comp, G5, F4, 8);
    comp_add_chord_2(comp, F5, F4, 8);
    comp_add_chord_2(comp, E5, F4, 8);
    comp_add_chord_2(comp, D5, F4, 8);
    comp_add_chord_2(comp, E5, C4, 4);
    comp_add_chord_2(comp, F5, C4, 8);
    comp_add_chord_2(comp, E5, C4, 8);
    comp_add_chord_2(comp, D5, C4, 8);
    comp_add_chord_2(comp, C5, C4, 8);
    comp_add_chord_2(comp, B4, G3, 4);
    comp_add_chord_2(comp, C5, G3, 8);
    comp_add_chord_2(comp, D5, G3, 8);
    comp_add_chord_2(comp, E5, G3, 8);
    comp_add_chord_2(comp, C5, G3, 8);
    comp_add_chord_2(comp, D5, G3, 4);
    comp_add_chord_1(comp, G3, 4);
    comp_add_chord_1(comp, B3, 4);

    comp_add_chord_2(comp, G5, C4, 4);
    comp_add_chord_2(comp, C5, C4, 8);
    comp_add_chord_2(comp, D5, C4, 8);
    comp_add_chord_2(comp, E5, C4, 8);
    comp_add_chord_2(comp, F5, C4, 8);
    comp_add_chord_2(comp, G5, C4, 4);
    comp_add_chord_2(comp, C5, C4, 4);
    comp_add_chord_2(comp, C5, E4, 4);

    comp_add_chord_2(comp, A5, F4, 4);
    comp_add_chord_2(comp, F5, F4, 8);
    comp_add_chord_2(comp, G5, F4, 8);
    comp_add_chord_2(comp, A5, F4, 8);
    comp_add_chord_2(comp, B5, F4, 8);
    comp_add_chord_2(comp, C6, C4, 4);
    comp_add_chord_2(comp, C5, C4, 4);
    comp_add_chord_2(comp, C5, E4, 4);

    comp_add_chord_2(comp, F5, F4, 4);
    comp_add_chord_2(comp, G5, F4, 8);
    comp_add_chord_2(comp, F5, F4, 8);
    comp_add_chord_2(comp, E5, F4, 8);
    comp_add_chord_2(comp, D5, F4, 8);
    comp_add_chord_2(comp, E5, C4, 4);
    comp_add_chord_2(comp, F5, C4, 8);
    comp_add_chord_2(comp, E5, C4, 8);
    comp_add_chord_2(comp, D5, C4, 8);
    comp_add_chord_2(comp, C5, C4, 8);
    comp_add_chord_2(comp, D5, G3, 4);
    comp_add_chord_2(comp, E5, G3, 8);
    comp_add_chord_2(comp, D5, G3, 8);
    comp_add_chord_2(comp, C5, G3, 8);
    comp_add_chord_2(comp, B4, G3, 8);
    comp_add_chord_2(comp, C5, C4, 2);
}

void add_all_star(comp_t *comp)
{
    comp_add_chord_2(comp,  F5,  F4, 4);
    comp_add_chord_2(comp,  C6,  F4, 8);
    comp_add_chord_2(comp,  A5,  A4, 8);
    comp_add_chord_2(comp,  A5,  C5, 4);
    comp_add_chord_2(comp,  G5,  G4, 8);
    comp_add_chord_2(comp,  F5,  A4, 8);
    comp_add_chord_2(comp,  F5,  F4, 8);
    comp_add_chord_2(comp, AS5,  G4, 4);
    comp_add_chord_2(comp,  A5,  A4, 8);
    comp_add_chord_2(comp,  A5,  A4, 8);
    comp_add_chord_2(comp,  G5, AS4, 8);
    comp_add_chord_2(comp,  G5, AS4, 8);
    comp_add_chord_2(comp,  F5,  D5, 8);
    comp_add_chord_2(comp,  F5,  D5, 8);
    comp_add_chord_2(comp,  C6,  A4, 4);
}
*/

void add_tetris(comp_t *comp)
{
    comp_add_chord_2(comp, TRBL,  E6, GS5, 4, false);
    comp_add_chord_2(comp, TRBL,  B5, GS5, 8, false);
    comp_add_chord_2(comp, TRBL,  C6,  A5, 8, false);
    comp_add_chord_2(comp, TRBL,  D6,  B5, 4, false);
    comp_add_chord_2(comp, TRBL,  C6,  A5, 8, false);
    comp_add_chord_2(comp, TRBL,  B5, GS5, 8, false);
    
    comp_add_chord_2(comp, TRBL,  A5,  E5, 4, false);
    comp_add_chord_2(comp, TRBL,  A5,  E5, 8, false);
    comp_add_chord_2(comp, TRBL,  C6,  A5, 8, false);
    comp_add_chord_2(comp, TRBL,  E6,  C6, 4, false);
    comp_add_chord_2(comp, TRBL,  D6,  B5, 8, false);
    comp_add_chord_2(comp, TRBL,  C6,  A5, 8, false);
    
    comp_add_chord_2(comp, TRBL,  B5, GS5, 4, false);
    comp_add_chord_2(comp, TRBL,  B5, GS5, 8, false);
    comp_add_chord_2(comp, TRBL,  C6,  A5, 8, false);
    comp_add_chord_2(comp, TRBL,  D6,  B5, 4, false);
    comp_add_chord_2(comp, TRBL,  E6,  C6, 4, false);
    
    comp_add_chord_2(comp, TRBL,  C6,  A5, 4, false);
    comp_add_chord_2(comp, TRBL,  A5,  E5, 4, false);
    comp_add_chord_2(comp, TRBL,  A5,  E5, 4, false);

    comp_add_rest(comp, TRBL, 4);

    comp_add_chord_2(comp, TRBL,  D6,  A5, 4, true);
    comp_add_chord_2(comp, TRBL,  F6,  D6, 8, false);
    comp_add_chord_2(comp, TRBL,  A6,  F6, 4, false);
    comp_add_chord_2(comp, TRBL,  G6,  E6, 8, false);
    comp_add_chord_2(comp, TRBL,  F6,  D6, 8, false);
    comp_add_chord_2(comp, TRBL,  E6,  C6, 4, true);
    comp_add_chord_2(comp, TRBL,  C6,  A5, 8, false);
    comp_add_chord_2(comp, TRBL,  E6,  C6, 4, false);
    comp_add_chord_2(comp, TRBL,  D6,  B5, 8, false);
    comp_add_chord_2(comp, TRBL,  C6,  A5, 8, false);
    comp_add_chord_2(comp, TRBL,  B5, GS5, 4, false);
    comp_add_chord_2(comp, TRBL,  B5, GS5, 8, false);
    comp_add_chord_2(comp, TRBL,  C6,  A5, 8, false);
    comp_add_chord_2(comp, TRBL,  D6,  B5, 4, false);
    comp_add_chord_2(comp, TRBL,  E6,  C6, 4, false);
    comp_add_chord_2(comp, TRBL,  C6,  A5, 4, false);
    comp_add_chord_2(comp, TRBL,  A5,  E5, 4, false);
    comp_add_chord_2(comp, TRBL,  A5,  E5, 4, false);

    

    comp_add_chord_1(comp, BASS,  E4, 8, false);
    comp_add_chord_1(comp, BASS, GS4, 8, false);
    comp_add_chord_1(comp, BASS,  B3, 8, false);
    comp_add_chord_1(comp, BASS, GS4, 8, false);
    comp_add_chord_1(comp, BASS,  E4, 8, false);
    comp_add_chord_1(comp, BASS, GS4, 8, false);
    comp_add_chord_1(comp, BASS,  B3, 8, false);
    comp_add_chord_1(comp, BASS, GS4, 8, false);
    
    comp_add_chord_1(comp, BASS,  A4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);
    comp_add_chord_1(comp, BASS,  E4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);
    comp_add_chord_1(comp, BASS,  A4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);
    comp_add_chord_1(comp, BASS,  E4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);

    comp_add_chord_1(comp, BASS,  E4, 8, false);
    comp_add_chord_1(comp, BASS, GS4, 8, false);
    comp_add_chord_1(comp, BASS,  B3, 8, false);
    comp_add_chord_1(comp, BASS, GS4, 8, false);
    comp_add_chord_1(comp, BASS,  E4, 8, false);
    comp_add_chord_1(comp, BASS, GS4, 8, false);
    comp_add_chord_1(comp, BASS,  B3, 8, false);
    comp_add_chord_1(comp, BASS, GS4, 8, false);

    comp_add_chord_1(comp, BASS,  A4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);
    comp_add_chord_1(comp, BASS,  E4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);
    comp_add_chord_1(comp, BASS,  A4, 8, false);

    comp_add_chord_1(comp, BASS,  A4, 8, false);
    comp_add_chord_1(comp, BASS,  B4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);
    
    comp_add_chord_1(comp, BASS,  D5, 8, false);
    comp_add_chord_1(comp, BASS,  F5, 8, false);
    comp_add_chord_1(comp, BASS,  A4, 8, false);
    comp_add_chord_1(comp, BASS,  F5, 8, false);
    comp_add_chord_1(comp, BASS,  D5, 8, false);
    comp_add_chord_1(comp, BASS,  F5, 8, false);
    comp_add_chord_1(comp, BASS,  A4, 8, false);
    comp_add_chord_1(comp, BASS,  F5, 8, false);

    comp_add_chord_1(comp, BASS,  A4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);
    comp_add_chord_1(comp, BASS,  E4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);
    comp_add_chord_1(comp, BASS,  A4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);
    comp_add_chord_1(comp, BASS,  E4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);

    comp_add_chord_1(comp, BASS,  E4, 8, false);
    comp_add_chord_1(comp, BASS, GS4, 8, false);
    comp_add_chord_1(comp, BASS,  B3, 8, false);
    comp_add_chord_1(comp, BASS, GS4, 8, false);
    comp_add_chord_1(comp, BASS,  E4, 8, false);
    comp_add_chord_1(comp, BASS, GS4, 8, false);
    comp_add_chord_1(comp, BASS,  B3, 8, false);
    comp_add_chord_1(comp, BASS, GS4, 8, false);

    comp_add_chord_1(comp, BASS,  A4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);
    comp_add_chord_1(comp, BASS,  E4, 8, false);
    comp_add_chord_1(comp, BASS,  C5, 8, false);
    comp_add_chord_1(comp, BASS,  A4, 4, false);
}

int main(int argc, char *argv[])
{
    comp_t *comp = comp_new(160);
    add_tetris(comp);

    wav_t *wav = wav_new_comp(44100, 1, 16, comp);
    wav_print_info(wav);
    wav_to_file(wav, "./tetris.wav");
    
    wav_free(wav);
    comp_free(comp);

    return 0;
}
