/*
 * Still hacked but with ring buffer
 */
#include <stdio.h>
#include <math.h>
#include <SDL.h>

#include "audioout.h"

unsigned int audioBufferSize = 0;
unsigned int outputAudioBufferSize = 0;
unsigned int sampleFrequency = 0;

static int buf_free(void) {
	int free = read_pos - write_pos - CHUNK_SIZE;
	if (free < 0) free += BUFFSIZE;
	return free;
}

static int buf_used(void) {
	int used = write_pos - read_pos;
	if (used < 0) used += BUFFSIZE;
	return used;
}

static int write_buffer(unsigned char* data,int len) {
	int first_len = BUFFSIZE - write_pos;
	int free = buf_free();
	if (len > free) len = free;
	if (first_len > len) first_len = len;
	/* till end of buffer */
	memcpy (&buffer[write_pos], data, first_len);
	if (len > first_len) { /* we have to wrap around */
		/* remaining part from beginning of buffer */
		memcpy (buffer, &data[first_len], len - first_len);
	}
	write_pos = (write_pos + len) % BUFFSIZE;
	return len;
}

static int read_buffer(unsigned char* data,int len) {
	int first_len = BUFFSIZE - read_pos;
	int buffered = buf_used();
	if (len > buffered) len = buffered;
	if (first_len > len) first_len = len;
	/* till end of buffer */
	memcpy (data, &buffer[read_pos], first_len);
	if (len > first_len) { /* we have to wrap around */
		/* remaining part from beginning of buffer */
		memcpy (&data[first_len], buffer, len - first_len);
	}
	read_pos = (read_pos + len) % BUFFSIZE;
	return len;
}

/* end ring buffer stuff */

void buffcallback(void *unused, Uint8 *stream, int len) {
	
	read_buffer(stream, len);
	/*printf("SDL: Full Buffers: %i\n", full_buffers); */
}

int sdlinit(void (*callback)(void *, Uint8 *, int), audioInfo *ai) {
	SDL_AudioSpec *desired, *obtained;
	
	if( SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO ) <0 ) {
		fprintf(stderr,"Unable to init SDL: %s\n", SDL_GetError());
		return -1;
	}
	
	/* setup audio */

	
	/* Allocate a desired SDL_AudioSpec */
	desired = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));
	
	/* Allocate space for the obtained SDL_AudioSpec */
	obtained = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));
	
	/* choose a samplerate and audio-format */
	desired->freq = 48000;
	desired->format = AUDIO_S8;
	
	/* Large audio buffers reduces risk of dropouts but increases response time.
	 *
	 * You should always check if you actually GOT the audiobuffer size you wanted,
	 * note that not hardware supports all buffer sizes (< 2048 bytes gives problems with some
	 * hardware). Older versions of SDL had a bug that caused many configuration to use a 
	 * buffersize of 11025 bytes, if your sdl.dll is approx. 1 Mb in stead of 220 Kb, download
	 * v1.2.8 of SDL or better...)
	 */
	desired->samples = 4096; 
	
	/* Our callback function */
	desired->callback=callback;
	desired->userdata=NULL;
	
	desired->channels = 1;
	
	/* Open the audio device and start playing sound! */
	if (SDL_OpenAudio(desired, obtained) < 0 ) {
		fprintf(stderr, "AudioMixer, Unable to open audio: %s\n", SDL_GetError());
		return -1;
	}
	
	ai->bufferSize = obtained->samples;
	ai->sampleRate = obtained->freq;
	
	/* if the format is 16 bit, two bytes are written for every sample */
	if (obtained->format==AUDIO_U16 || obtained->format==AUDIO_S16) {
		outputAudioBufferSize = 2*audioBufferSize;
	} else {
		outputAudioBufferSize = audioBufferSize;
	}   
	
	SDL_PauseAudio(0);
	
	free(desired);
	free(obtained);

	return 0;
}

