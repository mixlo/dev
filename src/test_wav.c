#include <stdio.h>
#include <comp.h>
#include <wav.h>

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

    comp_t *comp = comp_new(120);
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
    comp_add_chord_2(comp, C5, C4, 4);

    wav_t *wav = wav_new_comp(44100, 1, 16, comp);
    wav_print_info(wav);
    wav_to_file(wav, "./test_write_2.wav");
    
    wav_free(wav);
    comp_free(comp);

    return 0;
}
