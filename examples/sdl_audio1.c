/*
 * Simple 2 frequency sin beep in SDL
 * Dirty hacked from sdl wiki
 * 
 * gcc $(pkg-config --cflags --libs sdl) sdl_audio.c
 */
#include <stdio.h>
#include <math.h>
#include <SDL.h>

unsigned int sampleFrequency = 0;
unsigned int audioBufferSize = 0;
unsigned int outputAudioBufferSize = 0;

unsigned int freq1 = 440;
unsigned int phase1 = 0;

float lFreq1 = 1;
unsigned int lPhase1 = 0;
float lVal1 = 0;

float lFreq2 = 0.7;
unsigned int lPhase2 = 0;
float lVal2 = 0;

float lFreq3 = 0.5;
unsigned int lPhase3 = 0;
float lVal3 = 0;

int range = 127;

float dreieck(int phase, float freq, float mod) {
	float out;

	mod = mod / 2 + 0.5;

	// das ur-dreieck
	//out = 2 * (float) phase / sampleFrequency * freq1) - 1; 

	if (phase < (float)sampleFrequency / freq * mod) {
		out = 2 * (float)phase / sampleFrequency * freq / mod - 1;
	} else {
		out = 1 - 2 * (float)(phase - mod * sampleFrequency / freq) / sampleFrequency * freq / (1-mod);
	}

	return out;
}

void example_mixaudio(void *unused, Uint8 *stream, int len) {
	int i;
	float outputValue;

	for (i=0;i<len;i++) {
				
		//outputValue = sin(phase1 * 2 * M_PI * freq1 / sampleFrequency); //sin
		
		outputValue = dreieck(phase1, freq1, lVal1);
		//outputValue = dreieck(0.5, phase1);
		//outputValue = dreieck(1, phase1);
		//outputValue = (phase1 < (float)sampleFrequency / freq1 / 2) ? 1 : -1; //rechteck

		outputValue *= range;
		outputValue *= 1;
		if (outputValue > 127) outputValue = 127;        // and clip the result
		if (outputValue < -128) outputValue = -128;      // this seems a crude method, but works very well
		
		stream[i] = outputValue;
		
		phase1++;
		phase1 %= sampleFrequency / freq1;

		lPhase1++;
		if (lPhase1 > sampleFrequency / lFreq1) lPhase1 = 0;
		lVal1 = sin(lPhase1 * 2 * M_PI * lFreq1 / sampleFrequency);

		lPhase2++;
		if (lPhase2 > sampleFrequency / lFreq2) lPhase2 = 0;
		lVal2 = 3 + 2 * dreieck(lPhase2, lFreq2, 0);
		lFreq1 = lVal2;

		lPhase3++;
		if (lPhase3 > sampleFrequency / lFreq3) lPhase3 = 0;
		lVal2 = 440 + 100 * dreieck(lPhase3, lFreq3, 0);
		freq1 = lVal2;


	}
}
int main(int argc, char *argv[])
{
	
	if( SDL_Init(SDL_INIT_TIMER | SDL_INIT_AUDIO ) <0 ) {
		printf("Unable to init SDL: %s\n", SDL_GetError());
		return 1;
	}
	
	/* setup audio */
	SDL_AudioSpec *desired, *obtained;
	
	/* Allocate a desired SDL_AudioSpec */
	desired = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));
	
	/* Allocate space for the obtained SDL_AudioSpec */
	obtained = (SDL_AudioSpec *) malloc(sizeof(SDL_AudioSpec));
	
	/* choose a samplerate and audio-format */
	desired->freq = 44100;
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
	desired->callback=example_mixaudio;
	desired->userdata=NULL;
	
	desired->channels = 1;
	
	/* Open the audio device and start playing sound! */
	if ( SDL_OpenAudio(desired, obtained) < 0 ) {
		fprintf(stderr, "AudioMixer, Unable to open audio: %s\n", SDL_GetError());
		exit(1);
	}
	
	audioBufferSize = obtained->samples;
	sampleFrequency = obtained->freq;
	
	/* if the format is 16 bit, two bytes are written for every sample */
	if (obtained->format==AUDIO_U16 || obtained->format==AUDIO_S16) {
		outputAudioBufferSize = 2*audioBufferSize;
	} else {
		outputAudioBufferSize = audioBufferSize;
	}   
	
	SDL_Surface *screen = SDL_SetVideoMode(200,200, 16, SDL_SWSURFACE);
	SDL_WM_SetCaption("Audio Example",0);
	
	SDL_PauseAudio(0);
	
	int running = 1;
	
	SDL_Event event;
	while (running) {
		while (SDL_PollEvent(&event)) {
			/* GLOBAL KEYS / EVENTS */
			switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						default:
							break;
					}
					break;
				case SDL_QUIT:
					running = 0;
					break;
			}
			SDL_Delay(1);
		}
		SDL_Delay(1);
	}
	SDL_Quit();
	return EXIT_SUCCESS;
}
