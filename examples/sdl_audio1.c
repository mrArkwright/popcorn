/*
 * Simple 2 frequency sin beep in SDL
 * Dirty hacked from sdl wiki
 * 
 * gcc $(pkg-config --cflags --libs sdl) sdl_audio1.c
 */
#include <stdio.h>
#include <math.h>
#include <SDL.h>

unsigned int sampleRate = 0;
unsigned int audioBufferSize = 0;
unsigned int outputAudioBufferSize = 0;

float m2pi = 2 * M_PI;

float freq1 = 440;
float phase1 = 0;
float val1 = 0;
float vol1 = 1;

float freq2 = 880;
float phase2 = 0;
float val2 = 0;
float vol2 = 0;

float lFreq1 = 0.8;
float lPhase1 = 0;
float lVal1 = 0;

float lFreq2 = 0.5;
float lPhase2 = 0;
float lVal2 = 0;

float lFreq3 = 5.1;
float lPhase3 = 0;
float lVal3 = 0;

int range = 127;

int debugCount = 0;
int debugLastCount = 0;
float debugOld;

void debug(float val, int maxCount) {
	if (debugCount < maxCount) {
		if (val != debugOld) {
			debugOld = val;
			printf("%d\t%.50f\n", debugLastCount, val);
			debugLastCount = 0;
			debugCount++;
		} else {
			debugLastCount++;
		}
	}
}

float oscSin(float *pPhase, float freq, float vol, float mod) {
	float out;
	float spp = sampleRate / freq; //samples per period
	float phase = *pPhase;

	*pPhase += 1;
	if (phase >= spp) *pPhase -= spp;

	mod = mod / 2 + 0.5;

	// der ur-sinus
    //out = sin(phase * 2 * M_PI * / spp);

	if (phase < spp * mod) {
		out = sin(phase * m2pi / spp / (2 * mod));
	} else {
		phase += spp * (1 - 2 * mod);
		out = sin(phase * m2pi / spp / (2 - 2 * mod));
	}

	out *= vol;

	return out;
}

float oscTri(float *pPhase, float freq, float vol, float mod) {
	float out;
	float spp = sampleRate / freq; //samples per period
	float phase = *pPhase;

	*pPhase += 1;
	if (phase >= spp) *pPhase -= spp;

	mod = mod / 2 + 0.5;

	// das ur-dreieck
	//out = 2 * (float)phase / spp - 1;

	if (phase < spp * mod) {
		out = 2 * phase / spp / mod - 1;
	} else {
		out = 1 - 2 * (phase - mod * spp) / spp / (1 - mod);
	}

	out *= vol;

	return out;
}

float oscRec(float *pPhase, float freq, float vol, float mod) {
	float out;
	float spp = sampleRate / freq; //samples per period
	float phase = *pPhase;

	*pPhase += 1;
	if (phase >= spp) *pPhase -= spp;

	mod = mod / 2 + 0.5;

	// das ur-rechteck
	//out = (phase < spp * 0.5) ? 1 : -1;

	out = (phase < spp * mod) ? 1 : -1;

	out *= vol;

	return out;
}

void example_mixaudio(void *unused, Uint8 *stream, int len) {
	int i;
	float outputValue;

	for (i=0;i<len;i++) {	
		
		val1 = oscSin(&phase1, freq1, vol1, 0);

		val2 = oscSin(&phase2, freq2, vol2, 0);

		outputValue = (val1 + val2) * range;
		if (outputValue > 127) outputValue = 127;        // and clip the result
		if (outputValue < -128) outputValue = -128;      // this seems a crude method, but works very well

		lVal1 = oscTri(&lPhase1, lFreq1, 1, 0);

		lVal2 = oscTri(&lPhase2, lFreq2, 1, 0);

		lVal3 = oscSin(&lPhase3, lFreq3, 1, -0.8);

		//routing
		vol1 = lVal1;

		stream[i] = outputValue;
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
	sampleRate = obtained->freq;
	
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
						case SDLK_SPACE:
							vol2 = !vol2;
							break;
						case SDLK_RETURN:
							vol1 = !vol1;
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
