/*
 * some simple oscillators with complex routing
 *
 * gcc $(pkg-config --cflags --libs sdl) -o osc oscillators.c && ./osc
 *
 */
#include <stdio.h>
#include <math.h>
#include <SDL.h>

#define A 8

unsigned int sampleRate = 0;
unsigned int audioBufferSize = 0;
unsigned int outputAudioBufferSize = 0;

typedef struct {
	float val;
	float *mod;
	float range;
} param;

typedef struct {
	char act;
	float val;
	float phase;
	float (*func)(float, float, float);
	param freq, vol, param1;
} osc;

osc oscs[A];

osc lfos[A];

float m2pi = 2 * M_PI;

int range = 127;
float *globalOutput;

int debugCount = 0;
int debugLastCount = 0;
float debugOld;

void init();
void initOsc(osc*, float(*)(float, float, float), float, float, float);
void initParam(param*, float*, float);

void example_mixaudio(void*, Uint8*, int);

void compOsc(osc*);
float compParam(param*);

float oscSin(float, float, float);
float oscTri(float, float, float);
float oscRec(float, float, float);

int main(int, char**);
void debug(float, int);

void init() { // configuration (routing and params)
	initOsc(&oscs[0], &oscRec, 440, 1, 0);
	initParam(&(oscs[0].freq), &(lfos[0].val), 10);
	initParam(&(oscs[0].param1), &(lfos[1].val), 0.9);
	initParam(&(oscs[0].vol), &(lfos[2].val), 0.9);

	initOsc(&lfos[0], &oscTri, 0.5, 1, 0);

	initOsc(&lfos[1], &oscSin, 0.4, 1, 0);

	initOsc(&lfos[2], &oscTri, 10, 1, -0.7);

	globalOutput = &(oscs[0].val);
}

void initOsc(osc *o, float (*func)(float, float, float), float freq, float vol, float param1) {
	o->act = 1;
	o->val = 0;
	o->phase = 0;
	o->func = func;

	o->freq.val = freq;
	o->freq.mod = NULL;

	o->vol.val = vol;
	o->vol.mod = NULL;

	o->param1.val = param1;
	o->param1.mod = NULL;
}

void initParam(param *p, float *mod, float range) {
	p->mod = mod;
	p->range = range;
}

void example_mixaudio(void *unused, Uint8 *stream, int len) {
	int i, j;
	float outputValue;

	for (i=0;i<len;i++) {
		
		for (j = 0; j < A; j++) {
			if (oscs[j].act == 1) {
				compOsc(&(oscs[j])); // compute all active oscillators
			}
		}

		for (j = 0; j < A; j++) {
			if (lfos[j].act == 1) {
				compOsc(&(lfos[j])); // compute all active lfo's
			}
		}

		outputValue = *globalOutput * range;

		if (outputValue > 127) outputValue = 127;        // and clip the result
		if (outputValue < -128) outputValue = -128;      // this seems a crude method, but works very well

		stream[i] = outputValue;
	}
}

void compOsc(osc *o) {
	float freq, vol, param1;
	float spp; // samples per period

	freq = compParam(&(o->freq));
	vol = compParam(&(o->vol));
	param1 = compParam(&(o->param1));

	spp = sampleRate / freq;
	o->phase++;
	if (o->phase >= spp) o->phase -= spp;

	o->val = o->func(o->phase, spp, param1) * vol;	
}

float compParam(param *p) {
	return p->val + ((p->mod != NULL) ? p->range * (*(p->mod)) : 0); // this is where the routing magic happens
}

float oscSin(float phase, float spp, float param1) {
	float out;

	param1 = param1 / 2 + 0.5;

	// der ur-sinus
    //out = sin(phase * 2 * M_PI * / spp);

	if (phase < spp * param1) {
		out = sin(phase * m2pi / spp / (2 * param1));
	} else {
		phase += spp * (1 - 2 * param1);
		out = sin(phase * m2pi / spp / (2 - 2 * param1));
	}

	return out;
}

float oscTri(float phase, float spp, float param1) {
	float out;

	param1 = param1 / 2 + 0.5;

	// das ur-dreieck
	//out = 2 * (float)phase / spp - 1;

	if (phase < spp * param1) {
		out = 2 * phase / spp / param1 - 1;
	} else {
		out = 1 - 2 * (phase - param1 * spp) / spp / (1 - param1);
	}

	return out;
}

float oscRec(float phase, float spp, float param1) {
	float out;

	param1 = param1 / 2 + 0.5;

	// das ur-rechteck
	//out = (phase < spp * 0.5) ? 1 : -1;

	out = (phase < spp * param1) ? 1 : -1;

	return out;
}

int main(int argc, char *argv[]) {
	init();

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
							break;
						case SDLK_RETURN:
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
