#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "wav.h"

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
static void write_delay(wav_t *wav, unsigned int sample_delay, FILE *fp);



// PUBLIC

wav_t *wav_new(char *filename)
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
    return wav->size;
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

    // Add delay and update size accordingly
    unsigned int sample_delay = wav->fmt->smpl_rate * wav->delay / 1000;
    wav->size += wav->fmt->blk_align * sample_delay;
    wav->data->size += wav->fmt->blk_align * sample_delay;

    // Change speed after adding delay
    wav->fmt->smpl_rate *= wav->speed;
    wav->fmt->byte_rate *= wav->speed;

    fwrite(riff_str,   1, 4, fp);
    fwrite(&wav->size, 4, 1, fp);
    fwrite(wave_str,   1, 4, fp);

    fwrite(fmt_str,              1, 4, fp);
    fwrite(&wav->fmt->size,      4, 1, fp);
    fwrite(&wav->fmt->audio_fmt, 2, 1, fp);
    fwrite(&wav->fmt->num_chnls, 2, 1, fp);
    fwrite(&wav->fmt->smpl_rate, 4, 1, fp);
    fwrite(&wav->fmt->byte_rate, 4, 1, fp);
    fwrite(&wav->fmt->blk_align, 2, 1, fp);
    fwrite(&wav->fmt->bps,       2, 1, fp);

    fwrite(data_str,         1, 4, fp);
    fwrite(&wav->data->size, 4, 1, fp);
    
    if (sample_delay)
	write_delay(wav, sample_delay, fp);

    // This call seems to crash sometimes depending on delay, e.g. 5000
    fwrite(wav->data->data, wav->data->size, 1, fp);

    fclose(fp);
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

static void write_delay(wav_t *wav, unsigned int sample_delay, FILE *fp)
{
    unsigned char *null_sample = calloc(wav->fmt->blk_align, sizeof *null_sample);

    for (int i = 0; i < sample_delay; i++)
	fwrite(null_sample, wav->fmt->blk_align, 1, fp);

    free(null_sample);
}
