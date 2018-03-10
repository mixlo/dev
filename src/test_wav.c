#include <stdio.h>
#include "wav.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
	printf("Usage: test <wav_file_path>\n");
	return 0;
    }

    wav_t *wav = wav_new(argv[1]);

    if (wav == NULL) {
	puts("Failed.");
	return 0;
    }

    printf("total size: %d\n", wav_get_size(wav));
    
    wav_set_speed(wav, 2);
    wav_set_delay(wav, 1000);
    wav_to_file(wav, "./test_write.wav");
    wav_free(wav);

    return 0;
}