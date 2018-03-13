#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <wav.h>

#define PI 3.14159265358979323846

static float FRQ[] = {
    130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08, 246.94,
    261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440.00, 466.16, 493.88,
    523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880.00, 932.33, 987.77
};

enum note {
    C3, CS3, D3, DS3, E3, F3, FS3, G3, GS3, A3, AS3, B3,
    C4, CS4, D4, DS4, E4, F4, FS4, G4, GS4, A4, AS4, B4,
    C5, CS5, D5, DS5, E5, F5, FS5, G5, GS5, A5, AS5, B5
};

typedef struct chunk_fmt chunk_fmt_t;
struct chunk_fmt
{
    unsigned int size;
    unsigned short audio_fmt;
    unsigned short num_chnls;
    unsigned int smpl_rate;
    unsigned int byte_rate;
    unsigned short blk_align;
    unsigned short bps;
};

typedef struct chunk_data chunk_data_t;
struct chunk_data
{
    unsigned int size;
    unsigned char *data;
};

struct wav
{
    unsigned int size;
    chunk_fmt_t *fmt;
    chunk_data_t *data;
    unsigned int delay;
    double speed;
};



// PRIVATE DECLARATIONS

static wav_t *wav_alloc(unsigned int size);
static chunk_fmt_t *fmt_alloc(
    unsigned int size,
    unsigned short audio_fmt,
    unsigned short num_chnls,
    unsigned int smpl_rate,
    unsigned int byte_rate,
    unsigned short blk_align,
    unsigned short bps);
static chunk_data_t *data_alloc(unsigned int size, unsigned char *data);
static wav_t *read_chunk_desc(FILE *fp);
static chunk_fmt_t *read_fmt_chunk(FILE *fp);
static chunk_data_t *read_data_chunk(FILE *fp);
static unsigned int get_sample_rate(wav_t *wav);
static unsigned int get_byte_rate(wav_t *wav);
static unsigned int get_data_size(wav_t *wav);
static unsigned long get_amplitude(wav_t *wav);
static void write_delay(wav_t *wav, FILE *fp);
static void write_test(wav_t *wav, FILE *fp);
static void write_scale(wav_t *wav, FILE *fp);
static void write_chord(wav_t *wav, FILE *fp);



// PUBLIC

wav_t *wav_new()
{
    return NULL;
}

wav_t *wav_new_comp()
{
    return NULL;
}

wav_t *wav_from_file(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    
    if (fp == NULL)
	return NULL;
    
    wav_t *wav = read_chunk_desc(fp);
    chunk_fmt_t *fmt = read_fmt_chunk(fp);
    chunk_data_t *data = read_data_chunk(fp);

    if (!wav || !fmt || !data) {
	free(wav);
	free(fmt);
	fclose(fp);
	return NULL;
    }
    
    wav->fmt = fmt;
    wav->data = data;

    fclose(fp);
    return wav;
}

void wav_free(wav_t *wav)
{
    free(wav->fmt);
    free(wav->data->data);
    free(wav->data);
    free(wav);
}

unsigned int wav_get_size(wav_t *wav)
{
    unsigned int sample_delay = wav->fmt->smpl_rate * wav->delay / 1000;
    return wav->size + wav->fmt->blk_align * sample_delay;
}

void wav_set_delay(wav_t *wav, unsigned int milliseconds)
{
    wav->delay = milliseconds;
}

void wav_set_speed(wav_t *wav, double factor)
{
    wav->speed = factor;
}

void wav_to_file(wav_t *wav, char *filename)
{
    FILE *fp = fopen(filename, "wb");

    char *riff_str = "RIFF";
    char *wave_str = "WAVE";
    char *fmt_str  = "fmt ";
    char *data_str = "data";

    unsigned int wav_size = wav_get_size(wav);
    unsigned int data_size = get_data_size(wav);
    unsigned int smpl_rate = get_sample_rate(wav);
    unsigned int byte_rate = get_byte_rate(wav);

    fwrite(riff_str,   1, 4, fp);
    fwrite(&wav_size,  4, 1, fp);
    fwrite(wave_str,   1, 4, fp);

    fwrite(fmt_str,              1, 4, fp);
    fwrite(&wav->fmt->size,      4, 1, fp);
    fwrite(&wav->fmt->audio_fmt, 2, 1, fp);
    fwrite(&wav->fmt->num_chnls, 2, 1, fp);
    fwrite(&smpl_rate,           4, 1, fp);
    fwrite(&byte_rate,           4, 1, fp);
    fwrite(&wav->fmt->blk_align, 2, 1, fp);
    fwrite(&wav->fmt->bps,       2, 1, fp);

    fwrite(data_str,   1, 4, fp);
    fwrite(&data_size, 4, 1, fp);
    
    if (wav->delay)
	write_delay(wav, fp);

    // This call seems to crash sometimes depending on delay, e.g. 5000
    //fwrite(wav->data->data, wav->data->size, 1, fp);
    //write_test(wav, fp);
    write_scale(wav, fp);
    //write_chord(wav, fp);


    fclose(fp);
}

void wav_print_info(wav_t *wav)
{
    printf("File size: %u bytes\n", wav_get_size(wav) + 8);
    printf("Audio format: %s\n", wav->fmt->audio_fmt == 1 ? "PCM" : "Unknown");
    printf("Sample rate: %u samples/second\n", get_sample_rate(wav));
    printf("Byte rate: %u bytes/second\n", get_byte_rate(wav));
    printf("Channels: %hu\n", wav->fmt->num_chnls);
    printf("Bits per sample: %hu bits\n", wav->fmt->bps);
    printf("Bits per sample frame: %hu bits\n", wav->fmt->blk_align * 8);
    printf("Samples data size: %u bytes\n", get_data_size(wav));
    printf("Number of samples: %lu\n",
	   (unsigned long)get_data_size(wav) /
	   (unsigned long)wav->fmt->blk_align);
    printf("Length: %.2f seconds\n",
	   (double)get_data_size(wav) /
	   (double)wav->fmt->blk_align /
	   (double)get_sample_rate(wav));
}



/* ##########   PRIVATE   ########## */

static wav_t *wav_alloc(unsigned int size)
{
    wav_t *wav = malloc(sizeof *wav);

    if (wav != NULL) {
	wav->size = size;
	wav->delay = 0;
	wav->speed = 1.0;
    }

    return wav;
}

static chunk_fmt_t *fmt_alloc(
    unsigned int size,
    unsigned short audio_fmt,
    unsigned short num_chnls,
    unsigned int smpl_rate,
    unsigned int byte_rate,
    unsigned short blk_align,
    unsigned short bps)
{
    chunk_fmt_t *chunk = malloc(sizeof *chunk);

    if (chunk != NULL) {
	chunk->size = size;
	chunk->audio_fmt = audio_fmt;
	chunk->num_chnls = num_chnls;
	chunk->smpl_rate = smpl_rate;
	chunk->byte_rate = byte_rate;
	chunk->blk_align = blk_align;
	chunk->bps = bps;
    }

    return chunk;
}

static chunk_data_t *data_alloc(unsigned int size, unsigned char *data)
{
    chunk_data_t *chunk = malloc(sizeof *chunk);

    if (chunk != NULL) {
	chunk->size = size;
	chunk->data = data;
    }
    
    return chunk;
}

static wav_t *read_chunk_desc(FILE *fp)
{
    char buffer[5];
    unsigned int size;

    if (fgets(buffer, 5, fp) == NULL || strcmp(buffer, "RIFF") != 0)
	return NULL;

    fread(&size, 4, 1, fp);
    
    if (fgets(buffer, 5, fp) == NULL || strcmp(buffer, "WAVE") != 0)
	return NULL;

    return wav_alloc(size);
}

static chunk_fmt_t *read_fmt_chunk(FILE *fp)
{
    char ID[5];
    unsigned int size;
    unsigned short audio_fmt;
    unsigned short num_chnls;
    unsigned int smpl_rate;
    unsigned int byte_rate;
    unsigned short blk_align;
    unsigned short bps;

    if (fgets(ID, 5, fp) == NULL || strcmp(ID, "fmt ") != 0)
	return NULL;

    fread(&size, 4, 1, fp);    
    fread(&audio_fmt, 2, 1, fp);
    fread(&num_chnls, 2, 1, fp);
    fread(&smpl_rate, 4, 1, fp);
    fread(&byte_rate, 4, 1, fp);
    fread(&blk_align, 2, 1, fp);
    fread(&bps, 2, 1, fp);

    if (size != 16 || audio_fmt != 1) // PCM check
	return NULL;

    return fmt_alloc(
	size,
        audio_fmt,
        num_chnls,
        smpl_rate,
        byte_rate,
        blk_align,
        bps
    );
}

static chunk_data_t *read_data_chunk(FILE *fp)
{
    char ID[5];
    unsigned int size;

    if (fgets(ID, 5, fp) == NULL || strcmp(ID, "data") != 0)
	return NULL;

    fread(&size, 4, 1, fp);

    unsigned char *data = malloc(sizeof *data * size);
    fread(data, size, 1, fp);

    return data_alloc(size, data);
}

static unsigned int get_sample_rate(wav_t *wav)
{
    return wav->fmt->smpl_rate * wav->speed;
}

static unsigned int get_byte_rate(wav_t *wav)
{
    return wav->fmt->byte_rate * wav->speed;
}

static unsigned int get_data_size(wav_t *wav)
{
    unsigned int sample_delay = wav->fmt->smpl_rate * wav->delay / 1000;
    return wav->data->size += wav->fmt->blk_align * sample_delay;
}

static unsigned long get_amplitude(wav_t *wav)
{
    return (unsigned long)pow(2, wav->fmt->bps - 1) - 1;
}

static void write_delay(wav_t *wav, FILE *fp)
{
    unsigned int sample_delay = wav->fmt->smpl_rate * wav->delay / 1000;
    unsigned char *null_sample = calloc(wav->fmt->blk_align, sizeof *null_sample);

    for (int i = 0; i < sample_delay; i++)
	fwrite(null_sample, wav->fmt->blk_align, 1, fp);

    free(null_sample);
}

static void write_test(wav_t *wav, FILE *fp)
{
    float cycles_per_second = 440; // A4 = 440 Hz
    float samples_per_second = 44100;
    float cycles_per_sample = cycles_per_second / samples_per_second;
    float radians_per_sample = cycles_per_sample * 2 * PI;

    // cycles_per_sample will be 0.01 here, meaning that it will take 100 samples to make 1 cycle
    // we convert it to radians to be able to use the sinus function to get the value of each
    // sample's position on the wave (phase)
    
    int total_samples = wav->data->size / wav->fmt->blk_align;
    float amp = 32000.0;
    
    short current_sample;

    for (int i = 0; i < total_samples; i++) {
	// for each sample, take one step further on the x-axis
	float xpos = radians_per_sample * i;
	// calculate the phase on the sine wave for that step
	float phase = sin(xpos);
	// multiply by amplitude to get full sample
	current_sample = (short)(amp * phase);
	// write sample to file
	fwrite(&current_sample, 2, 1, fp);
    }
}

static void write_scale(wav_t *wav, FILE *fp)
{
    // A3 to A5
//    int scale_length = 25;
//    float tones[] = {
//	220.00, 233.08, 246.94, 261.63, 277.18,
//	293.66, 311.13, 329.63, 349.23, 369.99,
//	392.00, 415.30, 440.00, 466.16, 493.88,
//	523.25, 554.37, 587.33, 622.25, 659.25,
//	698.46, 739.99, 783.99, 830.61, 880.00
//    };

    int scale_length = 16;
    float tones[] = {
	FRQ[C5], FRQ[D5], FRQ[C5], FRQ[E5], FRQ[C5], FRQ[F5], FRQ[C5], FRQ[G5],
	FRQ[C5], FRQ[F5], FRQ[C5], FRQ[E5], FRQ[C5], FRQ[D5], FRQ[C5], FRQ[C5]
    };

    float samples_per_second = wav->fmt->smpl_rate;
    float radians_per_sample[scale_length];
    for (int i = 0; i < scale_length; i++)
	radians_per_sample[i] = tones[i] * 2 * PI / samples_per_second;

    int total_samples = wav->data->size / wav->fmt->blk_align;
    float amp = (float)get_amplitude(wav);
    
    float phase = 0;
    short current;
    int scale_index = 0;

    for (int i = 0; i < total_samples; i++) {
	scale_index = (int)(i*scale_length/total_samples);
	phase = sin(radians_per_sample[scale_index] * i);
	current = (short)(amp * phase);
	fwrite(&current, 2, 1, fp);
    }
}

static void write_chord(wav_t *wav, FILE *fp)
{
    float tones[] = {FRQ[E4], FRQ[G4], FRQ[C5]};
    int chord_length = 3;

    float samples_per_second = wav->fmt->smpl_rate;
    float radians_per_sample[chord_length];
    for (int i = 0; i < chord_length; i++)
	radians_per_sample[i] = tones[i] * 2 * PI / samples_per_second;
    
    int total_samples = wav->data->size / wav->fmt->blk_align;
    float amp = (float)get_amplitude(wav);
    
    short current_sample;

    for (int i = 0; i < total_samples; i++) {
	float chord_phase = 0;
	for (int j = 0; j < chord_length; j++)
	    chord_phase += sin(radians_per_sample[j] * i);
	current_sample = (short)((amp / chord_length) * chord_phase);
	fwrite(&current_sample, 2, 1, fp);
    }
}

