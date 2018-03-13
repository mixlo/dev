#ifndef __WAV_H__
#define __WAV_H__

#include <stdbool.h>

typedef struct wav wav_t;

wav_t *wav_new();
wav_t *wav_new_comp();
wav_t *wav_from_file(char *filename);
void wav_to_file(wav_t *wav, char *filename);
void wav_set_delay(wav_t *wav, unsigned int milliseconds);
void wav_set_speed(wav_t *wav, double factor);
void wav_print_info(wav_t *wav);
void wav_free(wav_t *wav);

#endif
