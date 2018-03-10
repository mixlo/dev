#ifndef __WAV_H__
#define __WAV_H__

#include <stdbool.h>

typedef struct wav wav_t;

wav_t *wav_new(char *filename);
void wav_free();

unsigned int wav_get_size(wav_t *wav);
void wav_set_delay(wav_t *wav, unsigned int milliseconds);
void wav_set_speed(wav_t *wav, double factor);
void wav_to_file(wav_t *wav, char *filename);

#endif
