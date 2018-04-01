#include <stdio.h>
#include <comp.h>
#include <wav.h>

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

void add_tetris(comp_t *comp)
{
    comp_add_chord_3(comp,  E6, GS5,  E4, 8);
    comp_add_chord_3(comp,  E6, GS5, GS4, 8);
    comp_add_chord_3(comp,  B5, GS5,  B3, 8);
    comp_add_chord_3(comp,  C6,  A5, GS4, 8);
    comp_add_chord_3(comp,  B5,  D6,  E4, 8);
    comp_add_chord_3(comp,  B5,  D6, GS4, 8);
    comp_add_chord_3(comp,  C6,  A5,  B3, 8);
    comp_add_chord_3(comp,  B5, GS5, GS4, 8);
    comp_add_chord_3(comp,  A5,  E5,  E4, 4);
}

int main(int argc, char *argv[])
{
    /*
    if (argc < 2) {
	printf("Usage: test <wav_file_path>\n");
	return 0;
    }

    wav_t *wav = wav_from_file(argv[1]);

    if (wav == NULL) {
	puts("Failed.");
	return 0;
    }

    wav_print_info(wav);
    //wav_set_speed(wav, 1);
    //wav_set_delay(wav, 1000);
    wav_to_file(wav, "./test_write.wav");
    wav_free(wav);
    */

    //comp_t *comp = comp_new(60);
    //comp_add_chord_2(comp, C5, E4, 1);
    
    //comp_t *comp = comp_new(160);
    //add_twinkle(comp);

    //comp_t *comp = comp_new(160);
    //add_something(comp);

    //comp_t *comp = comp_new(120);
    //add_all_star(comp);

    comp_t *comp = comp_new(120);
    add_tetris(comp);

    wav_t *wav = wav_new_comp(44100, 1, 16, comp);
    wav_print_info(wav);
    wav_to_file(wav, "./tetris.wav");
    
    wav_free(wav);
    comp_free(comp);

    return 0;
}
