#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <llist.h>
#include <comp.h>
#include <wav.h>

#define PI 3.14159265358979323846

typedef struct chunk_fmt chunk_fmt_t;
typedef struct chunk_data chunk_data_t;

struct wav
{
    chunk_fmt_t  *fmt;
    chunk_data_t *data;
    unsigned int  delay;
    double        speed;
};

struct chunk_fmt
{
    unsigned short audio_fmt;
    unsigned short num_chnls;
    unsigned int   smpl_rate;
    unsigned short blk_align;
    unsigned short bps;
};

struct chunk_data
{
    llist_t *samples;
};


/* ##########   PRIVATE DECLARATIONS   ########## */

static wav_t *wav_alloc();
static chunk_fmt_t *fmt_alloc(
    unsigned short audio_fmt,
    unsigned short num_chnls,
    unsigned int   smpl_rate,
    unsigned short blk_align,
    unsigned short bps);
static chunk_data_t *data_alloc(llist_t *samples);

static double get_quad_func_a(
    double vx,
    double vy,
    double px,
    double py);
static void add_chord_sample(
    wav_t *wav,
    int chord_size,
    float *radians_per_note,
    unsigned long sine_wave_idx,
    double amplitude);
static void add_chord_samples_adsr(
    wav_t *wav,
    unsigned long num_samples,
    int chord_size,
    float *radians_per_note,
    double amplitude);
static void add_samples_by_comp(wav_t *wav, comp_t *comp);

static wav_t *read_desc_chunk(FILE *fp);
static chunk_fmt_t *read_fmt_chunk(FILE *fp);
static chunk_data_t *read_data_chunk(FILE *fp, unsigned short sample_size);
static void write_delay(wav_t *wav, FILE *fp);

static unsigned int get_fmt_size(wav_t *wav);
static unsigned int get_data_size(wav_t *wav);
static unsigned int get_num_samples(wav_t *wav);
static unsigned int get_num_delay_samples(wav_t *wav);
static unsigned int get_sample_rate(wav_t *wav);
static unsigned int get_byte_rate(wav_t *wav);
static unsigned long get_amplitude(wav_t *wav);
static float get_freq_rad(note_t note);


/* ##########   PUBLIC   ########## */

wav_t *wav_new(unsigned int smpl_rate, unsigned short num_chnls, unsigned short bps)
{
    wav_t *wav = wav_alloc();
    if (wav == NULL) {
	return NULL;
    }

    chunk_fmt_t *fmt = fmt_alloc(1, num_chnls, smpl_rate, num_chnls * bps / 8, bps);
    if (fmt == NULL) {
	free(wav);
	return NULL;
    }
    
    llist_t *samples = llist_new(&free);
    if (samples == NULL) {
	free(wav);
	free(fmt);
	return NULL;
    }

    // data_alloc() frees samples if it fails
    chunk_data_t *data = data_alloc(samples);
    if (data == NULL) {
	free(wav);
	free(fmt);
	return NULL;
    }
    
    wav->fmt = fmt;
    wav->data = data;
    
    return wav;
}

wav_t *wav_new_comp(unsigned smpl_rate, unsigned short num_chnls, unsigned short bps, comp_t *comp)
{
    wav_t *wav = wav_new(smpl_rate, num_chnls, bps);
    add_samples_by_comp(wav, comp);
    return wav;
}

wav_t *wav_from_file(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    
    if (fp == NULL)
	return NULL;

    wav_t *wav = read_desc_chunk(fp);
    if (wav == NULL) {
	fclose(fp);
	return NULL;
    }

    chunk_fmt_t *fmt = read_fmt_chunk(fp);
    if (fmt == NULL) {
	free(wav);
	fclose(fp);
	return NULL;
    }

    chunk_data_t *data = read_data_chunk(fp, fmt->blk_align);
    if (data == NULL) {
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
    llist_free(wav->data->samples);
    free(wav->data);
    free(wav);
}

unsigned int wav_get_size(wav_t *wav)
{
    unsigned int wave_tag_size = 4;
    unsigned int fmt_chunk_size = get_fmt_size(wav) + 8;
    unsigned int data_chunk_size = get_data_size(wav) + 8;
    return wave_tag_size + fmt_chunk_size + data_chunk_size;
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
    unsigned int fmt_size = get_fmt_size(wav);
    unsigned int data_size = get_data_size(wav);
    unsigned int smpl_rate = get_sample_rate(wav);
    unsigned int byte_rate = get_byte_rate(wav);

    fwrite(riff_str,   1, 4, fp);
    fwrite(&wav_size,  4, 1, fp);
    fwrite(wave_str,   1, 4, fp);

    fwrite(fmt_str,              1, 4, fp);
    fwrite(&fmt_size,            4, 1, fp);
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

    llist_iter_t *sample_iter = llist_get_iter(wav->data->samples);
    unsigned char *current_sample = NULL;
    
    while ((current_sample = llist_iter_next(sample_iter)))
	fwrite(current_sample, wav->fmt->blk_align, 1, fp);
    
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

// Takes a pointer to an entire sample frame, including all channels
void wav_add_sample(wav_t *wav, void *sample)
{
    void *new_sample = malloc(wav->fmt->blk_align);
    memcpy(new_sample, sample, wav->fmt->blk_align);
    llist_append(wav->data->samples, new_sample);
}



/* ##########   PRIVATE   ########## */

static wav_t *wav_alloc()
{
    wav_t *wav = malloc(sizeof *wav);

    if (wav != NULL) {
	wav->fmt = NULL;
	wav->data = NULL;
	wav->delay = 0;
	wav->speed = 1.0;
    }

    return wav;
}

static chunk_fmt_t *fmt_alloc(
    unsigned short audio_fmt,
    unsigned short num_chnls,
    unsigned int smpl_rate,
    unsigned short blk_align,
    unsigned short bps)
{
    chunk_fmt_t *chunk = malloc(sizeof *chunk);

    if (chunk != NULL) {
	chunk->audio_fmt = audio_fmt;
	chunk->num_chnls = num_chnls;
	chunk->smpl_rate = smpl_rate;
	chunk->blk_align = blk_align;
	chunk->bps = bps;
    }

    return chunk;
}

static chunk_data_t *data_alloc(llist_t *samples)
{
    chunk_data_t *chunk = malloc(sizeof *chunk);

    if (chunk != NULL) {
	chunk->samples = samples;
    } else {
	llist_free(samples);
    }

    return chunk;
}

// Get the leading coefficient (a-value) of a quadratic function, by its
// vertex coordinates and the coordinates of an arbitrary point on the curve.
// By quadratic function vertex form:
//  y = a * (x - h)^2 + k
// py = a * (px - vx) + vy
//  a = (py - vy) / (px - vx)^2
static double get_quad_func_a(
    double vx,
    double vy,
    double px,
    double py)
{
    double x_diff = px - vx;
    double y_diff = py - vy;
    return y_diff / (x_diff * x_diff);
}

static void add_chord_sample(
    wav_t *wav,
    int chord_size,
    float *radians_per_note,
    unsigned long sine_wave_idx,
    double amplitude)
{
    // Support for 32 bit samples
    long current_sample;

    // Sound waves for chords are the sum of the single note frequenzies
    double sample_phase = 0;
    for (int n = 0; n < chord_size; n++)
	sample_phase += sin(radians_per_note[n] * sine_wave_idx);

    // sin() returns within range [-1, 1], but the sum for all notes will
    // be in range [-chord_size, chord_size], so amplitude us divided by
    // chord_size to make sure sample size doesn't exceed bits per sample
    current_sample = (amplitude / chord_size) * sample_phase;

    // Currently, same sample in all channels
    unsigned char *sample_frame = malloc(sizeof *sample_frame * wav->fmt->blk_align);
    for (int c = 0; c < wav->fmt->num_chnls; c++)
	memcpy(sample_frame + wav->fmt->blk_align * c, &current_sample, wav->fmt->bps / 8);
	    
    llist_append(wav->data->samples, sample_frame);
}

// Use quadratic functions to generate the amplitude between each sample.
// Making sure that the amplitude change factor reaches 0 between phases of ADSR
// generates a continuous curve, to smoothen the transitions between each phase.
// This avoids the "popping" sound appearing for the transition between each phase
// when applying linear change in amplitude.
// The amplitude curve of each phase is represented by two quadratic functions
// with the same leading coeffeicient, although negated, meeting each other half
// way through the x-range of the phase.
// Simply put, one function is used to represent the entire phase curve, but
// it's "flipped" in the middle.
static void add_chord_samples_adsr(
    wav_t *wav,
    unsigned long num_samples,
    int chord_size,
    float *radians_per_note,
    double amplitude)
{
    // Get the x-coordinate of the vertex of the quadratic function of each part of ADSR
    unsigned long attack_start_vx  = 0;
    unsigned long attack_end_vx    = num_samples * 0.10; // 10% attack
    unsigned long decay_start_vx   = attack_end_vx;
    unsigned long decay_end_vx     = num_samples * 0.20; // 10% decay
    unsigned long sustain_start_vx = decay_end_vx;
    unsigned long sustain_end_vx   = num_samples * 0.90; // 70% sustain
    unsigned long release_start_vx = sustain_end_vx;
    unsigned long release_end_vx   = num_samples;        // 10% release

    // Get the y-coordinate of the vertex of the quadratic function of each part of ADSR
    unsigned long attack_start_vy  = 0;
    unsigned long attack_end_vy    = amplitude;        // attack peaks volume to 100%
    unsigned long decay_start_vy   = attack_end_vy;
    unsigned long decay_end_vy     = amplitude * 0.70; // decay lowers by 30%
    unsigned long sustain_start_vy = decay_end_vy;
    unsigned long sustain_end_vy   = sustain_start_vy; // constant sustain
    unsigned long release_start_vy = sustain_end_vy;
    unsigned long release_end_vy   = 0;                // release lowers volume to 0

    // Get the x-coordinate of the point halfway through the curve of each part of ADSR
    unsigned long attack_half_px  = (attack_start_vx + attack_end_vx) / 2;
    unsigned long decay_half_px   = (decay_start_vx + decay_end_vx) / 2;
    unsigned long sustain_half_px = (sustain_start_vx + sustain_end_vx) / 2;
    unsigned long release_half_px = (release_start_vx + release_end_vx) / 2;

    // Get the y-coordinate of the point halfway through the curve of each part of ADSR
    unsigned long attack_half_py  = (attack_start_vy + attack_end_vy) / 2;
    unsigned long decay_half_py   = (decay_start_vy + decay_end_vy) / 2;
    unsigned long sustain_half_py = (sustain_start_vy + sustain_end_vy) / 2;
    unsigned long release_half_py = (release_start_vy + release_end_vy) / 2;

    // Get the absolute value of the leading coefficient of each ADSR curve
    double attack_a = fabs(get_quad_func_a(
	attack_start_vx,
	attack_start_vy,
	attack_half_px,
	attack_half_py));
    double decay_a = fabs(get_quad_func_a(
	decay_start_vx,
	decay_start_vy,
	decay_half_px,
	decay_half_py));
    double sustain_a = fabs(get_quad_func_a(
	sustain_start_vx,
	sustain_start_vy,
	sustain_half_px,
	sustain_half_py));
    double release_a = fabs(get_quad_func_a(
	release_start_vx,
	release_start_vy,
	release_half_px,
	release_half_py));

    // Add samples for both halves of each phase of ADSR
    double current_amp = attack_start_vy;
    long x_diff;
    unsigned long x;

    // Attack
    for (x = attack_start_vx; x < attack_half_px; x++) {
	add_chord_sample(wav, chord_size, radians_per_note, x, current_amp);
	x_diff = x - attack_start_vx;
	current_amp = attack_a * x_diff * x_diff + attack_start_vy;
    }

    for (x = attack_half_px; x < attack_end_vx; x++) {
	add_chord_sample(wav, chord_size, radians_per_note, x, current_amp);
	x_diff = x - attack_end_vx;
	current_amp = -attack_a * x_diff * x_diff + attack_end_vy;
    }

    // Decay
    for (x = decay_start_vx; x < decay_half_px; x++) {
	add_chord_sample(wav, chord_size, radians_per_note, x, current_amp);
	x_diff = x - decay_start_vx;
	current_amp = -decay_a * x_diff * x_diff + decay_start_vy;
    }
    
    for (x = decay_half_px; x < decay_end_vx; x++) {
	add_chord_sample(wav, chord_size, radians_per_note, x, current_amp);
	x_diff = x - decay_end_vx;
	current_amp = decay_a * x_diff * x_diff + decay_end_vy;
    }

    // Sustain
    for (x = sustain_start_vx; x < sustain_half_px; x++) {
	add_chord_sample(wav, chord_size, radians_per_note, x, current_amp);
	x_diff = x - sustain_start_vx;
	current_amp = -sustain_a * x_diff * x_diff + sustain_start_vy;
    }

    for (x = sustain_half_px; x < sustain_end_vx; x++) {
	add_chord_sample(wav, chord_size, radians_per_note, x, current_amp);
	x_diff = x - sustain_end_vx;
	current_amp = sustain_a * x_diff * x_diff + sustain_end_vy;
    }

    // Release
    for (x = release_start_vx; x < release_half_px; x++) {
	add_chord_sample(wav, chord_size, radians_per_note, x, current_amp);
	x_diff = x - release_start_vx;
	current_amp = -release_a * x_diff * x_diff + release_start_vy;
    }

    for (x = release_half_px; x < release_end_vx; x++) {
	add_chord_sample(wav, chord_size, radians_per_note, x, current_amp);
	x_diff = x - release_end_vx;
	current_amp = release_a * x_diff * x_diff + release_end_vy;
    }
}

static void add_samples_by_comp(wav_t *wav, comp_t *comp)
{
    double samples_per_second = get_sample_rate(wav);
    double amplitude = get_amplitude(wav);

    // Support for 10 notes per chord
    float radians_per_note[10];

    llist_iter_t *chord_iter = comp_get_chords_iterator(comp);
    chord_t *current_chord = NULL;
    
    while ((current_chord = llist_iter_next(chord_iter))) {
	int chord_size = comp_get_chord_size(current_chord);
	note_t *chord_notes = comp_get_chord_notes(current_chord);

	// Calculate radians per sample for each individual note
	for (int n = 0; n < chord_size; n++)
	    radians_per_note[n] = get_freq_rad(chord_notes[n]) / samples_per_second;

	// Calculate how many samples needed for this chord, depending on
	// sample rate, bpm and chord duration
	double chord_seconds = comp_get_chord_duration(comp, current_chord);
	unsigned long num_samples = samples_per_second * chord_seconds;

	//add_chord_samples_adsr(wav, num_samples, chord_size, radians_per_note, amplitude);
	add_chord_samples_adsr(wav, num_samples, chord_size, radians_per_note, amplitude);
    }
}

static wav_t *read_desc_chunk(FILE *fp)
{
    char buffer[5];
    unsigned int size;

    if (fgets(buffer, 5, fp) == NULL || strcmp(buffer, "RIFF") != 0)
	return NULL;

    fread(&size, 4, 1, fp);
    
    if (fgets(buffer, 5, fp) == NULL || strcmp(buffer, "WAVE") != 0)
	return NULL;

    return wav_alloc();
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

    return fmt_alloc(audio_fmt, num_chnls, smpl_rate, blk_align, bps);
}

static chunk_data_t *read_data_chunk(FILE *fp, unsigned short sample_size)
{
    char ID[5];
    unsigned int size;

    if (fgets(ID, 5, fp) == NULL || strcmp(ID, "data") != 0)
	return NULL;

    fread(&size, 4, 1, fp);

    llist_t *samples = llist_new(&free);

    if (samples == NULL)
	return NULL;

    unsigned int num_samples = size / sample_size;
    for (int i = 0; i < num_samples; i++) {
	void *sample = malloc(sample_size);
	fread(sample, sample_size, 1, fp);
	llist_append(samples, sample);
    }

    return data_alloc(samples);
}

static void write_delay(wav_t *wav, FILE *fp)
{
    unsigned int num_delay_samples = get_num_delay_samples(wav);
    void *null_sample = calloc(1, wav->fmt->blk_align);

    for (int i = 0; i < num_delay_samples; i++)
	fwrite(null_sample, wav->fmt->blk_align, 1, fp);

    free(null_sample);
}

static unsigned int get_num_samples(wav_t *wav)
{
    return llist_length(wav->data->samples);
}

static unsigned int get_num_delay_samples(wav_t *wav)
{
    return get_sample_rate(wav) * wav->delay / 1000;
}

static unsigned int get_fmt_size(wav_t *wav)
{
    return wav->fmt->audio_fmt == 1 ? 16 : -1; // 16 bytes for PCM
}

static unsigned int get_data_size(wav_t *wav)
{
    return wav->fmt->blk_align * (get_num_samples(wav) + get_num_delay_samples(wav));
}

static unsigned int get_sample_rate(wav_t *wav)
{
    return wav->fmt->smpl_rate * wav->speed;
}

static unsigned int get_byte_rate(wav_t *wav)
{
    return get_sample_rate(wav) * wav->fmt->blk_align;
}

static unsigned long get_amplitude(wav_t *wav)
{
    return pow(2, wav->fmt->bps - 1) - 1;
}

static float FRQ_RAD[] = {
     130.81 * 2 * PI,  138.59 * 2 * PI,  146.83 * 2 * PI,  155.56 * 2 * PI,
     164.81 * 2 * PI,  174.61 * 2 * PI,  185.00 * 2 * PI,  196.00 * 2 * PI,
     207.65 * 2 * PI,  220.00 * 2 * PI,  233.08 * 2 * PI,  246.94 * 2 * PI,
     261.63 * 2 * PI,  277.18 * 2 * PI,  293.66 * 2 * PI,  311.13 * 2 * PI,
     329.63 * 2 * PI,  349.23 * 2 * PI,  369.99 * 2 * PI,  392.00 * 2 * PI,
     415.30 * 2 * PI,  440.00 * 2 * PI,  466.16 * 2 * PI,  493.88 * 2 * PI,
     523.25 * 2 * PI,  554.37 * 2 * PI,  587.33 * 2 * PI,  622.25 * 2 * PI,
     659.25 * 2 * PI,  698.46 * 2 * PI,  739.99 * 2 * PI,  783.99 * 2 * PI,
     830.61 * 2 * PI,  880.00 * 2 * PI,  932.33 * 2 * PI,  987.77 * 2 * PI,
    1046.50 * 2 * PI, 1108.73 * 2 * PI, 1174.66 * 2 * PI, 1244.51 * 2 * PI,
    1318.51 * 2 * PI, 1396.91 * 2 * PI, 1479.98 * 2 * PI, 1567.98 * 2 * PI,
    1661.22 * 2 * PI, 1760.00 * 2 * PI, 1864.66 * 2 * PI, 1975.73 * 2 * PI
};

static float get_freq_rad(note_t note)
{
    switch (note) {
    case C3 : return FRQ_RAD[ 0];
    case CS3: return FRQ_RAD[ 1];
    case D3 : return FRQ_RAD[ 2];
    case DS3: return FRQ_RAD[ 3];
    case E3 : return FRQ_RAD[ 4];
    case F3 : return FRQ_RAD[ 5];
    case FS3: return FRQ_RAD[ 6];
    case G3 : return FRQ_RAD[ 7];
    case GS3: return FRQ_RAD[ 8];
    case A3 : return FRQ_RAD[ 9];
    case AS3: return FRQ_RAD[10];
    case B3 : return FRQ_RAD[11];
    case C4 : return FRQ_RAD[12];
    case CS4: return FRQ_RAD[13];
    case D4 : return FRQ_RAD[14];
    case DS4: return FRQ_RAD[15];
    case E4 : return FRQ_RAD[16];
    case F4 : return FRQ_RAD[17];
    case FS4: return FRQ_RAD[18];
    case G4 : return FRQ_RAD[19];
    case GS4: return FRQ_RAD[20];
    case A4 : return FRQ_RAD[21];
    case AS4: return FRQ_RAD[22];
    case B4 : return FRQ_RAD[23];
    case C5 : return FRQ_RAD[24];
    case CS5: return FRQ_RAD[25];
    case D5 : return FRQ_RAD[26];
    case DS5: return FRQ_RAD[27];
    case E5 : return FRQ_RAD[28];
    case F5 : return FRQ_RAD[29];
    case FS5: return FRQ_RAD[30];
    case G5 : return FRQ_RAD[31];
    case GS5: return FRQ_RAD[32];
    case A5 : return FRQ_RAD[33];
    case AS5: return FRQ_RAD[34];
    case B5 : return FRQ_RAD[35];
    case C6 : return FRQ_RAD[36];
    case CS6: return FRQ_RAD[37];
    case D6 : return FRQ_RAD[38];
    case DS6: return FRQ_RAD[39];
    case E6 : return FRQ_RAD[40];
    case F6 : return FRQ_RAD[41];
    case FS6: return FRQ_RAD[42];
    case G6 : return FRQ_RAD[43];
    case GS6: return FRQ_RAD[44];
    case A6 : return FRQ_RAD[45];
    case AS6: return FRQ_RAD[46];
    case B6 : return FRQ_RAD[47];
    default:  return 0.00;
    }
}

/*
static void write_test(wav_t *wav, FILE *fp)
{
    double cycles_per_second = 440; // A4 = 440 Hz
    double samples_per_second = get_sample_rate(wav);
    double cycles_per_sample = cycles_per_second / samples_per_second;
    double radians_per_sample = cycles_per_sample * 2 * PI;

    // cycles_per_sample will be 0.01 here, meaning that it will take 100 samples to make 1 cycle
    // we convert it to radians to be able to use the sinus function to get the value of each
    // sample's position on the wave (phase)
    
    unsigned int total_samples = get_num_samples(wav);
    unsigned long amp = get_amplitude(wav);

    // support for 32 bit samples
    long current_sample;

    for (int i = 0; i < total_samples; i++) {
	// for each sample, take one step further on the x-axis
	double xpos = radians_per_sample * i;
	// calculate the phase on the sine wave for that step
	double phase = sin(xpos);
	// multiply by amplitude to get full sample
	current_sample = amp * phase;
	// write same sample to file for each channel
	for (int j = 0; j < wav->fmt->num_chnls; j++) {
	    fwrite(&current_sample, wav->fmt->bps / 8, 1, fp);
	}
    }
}
*/
