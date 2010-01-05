/*
 *  main program
 *
 *  for now:
 *  - init audio
 *  - fill buffer
 */

#include <stdio.h>
#include <pthread.h>
#include <SDL.h>

/*#include "oscillators.h"*/
#include "mixAudio.h"
#include "audioout.h"

int running = 1;
unsigned int sampleRate;

int main(int argc, char *argv[]) {
	extern int running;
	ao_sample_format aoFormat;
	ao_device *aoDevice;
	aoMixParam aoMixParameters;
	pthread_t aoThread;
	int rc;

	/* UI Foo */
	SDL_Surface *screen;
	SDL_Event event;
	screen = SDL_SetVideoMode(200,200, 16, SDL_SWSURFACE);
	SDL_WM_SetCaption("Audio Example",0);
	/* UI Foo End */

	/* setting up voices */
	initVoices();

	/* libao */
	aoFormat.bits = 16;
	aoFormat.channels = 2;
	sampleRate = aoFormat.rate = 44100;
	aoFormat.byte_format = AO_FMT_LITTLE;

	aoDevice = aoInit(&aoFormat); /* error checks inside */

	aoMixParameters.dev = aoDevice;
	aoMixParameters.fmt = &aoFormat;
	rc = pthread_create(&aoThread, NULL, aoMix, (void *)&aoMixParameters);
	if(rc) {
		fprintf(stderr, "Couldn't start aoMix() thread, returned: %d\n", rc);
		running = 0;
	}


	/* OLD
	 * passing callback to sdlInit()
	sdlinit(&mixAudio, &aInfo);
	sampleRate = aInfo.sampleRate;
	*/

	/* More UI Foo */
	while (running) {
		while (SDL_PollEvent(&event)) {
			/* GLOBAL KEYS / EVENTS */
			switch (event.type) {
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = 0;
							break;
						case SDLK_a:
							playSound(60, 128);
							break;
						case SDLK_w:
							playSound(61, 128);
							break;
						case SDLK_s:
							playSound(62, 128);
							break;
						case SDLK_e:
							playSound(63, 128);
							break;
						case SDLK_d:
							playSound(64, 128);
							break;
						case SDLK_f:
							playSound(65, 128);
							break;
						case SDLK_t:
							playSound(66, 128);
							break;
						case SDLK_g:
							playSound(67, 128);
							break;
						case SDLK_z:
							playSound(68, 128);
							break;
						case SDLK_h:
							playSound(69, 128);
							break;
						case SDLK_u:
							playSound(70, 128);
							break;
						case SDLK_j:
							playSound(71, 128);
							break;
						case SDLK_k:
							playSound(72, 128);
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
						case SDLK_a:
							stopSound(60, 128);
							break;
						case SDLK_w:
							stopSound(61, 128);
							break;
						case SDLK_s:
							stopSound(62, 128);
							break;
						case SDLK_e:
							stopSound(63, 128);
							break;
						case SDLK_d:
							stopSound(64, 128);
							break;
						case SDLK_f:
							stopSound(65, 128);
							break;
						case SDLK_t:
							stopSound(66, 128);
							break;
						case SDLK_g:
							stopSound(67, 128);
							break;
						case SDLK_z:
							stopSound(68, 128);
							break;
						case SDLK_h:
							stopSound(69, 128);
							break;
						case SDLK_u:
							stopSound(70, 128);
							break;
						case SDLK_j:
							stopSound(71, 128);
							break;
						case SDLK_k:
							stopSound(72, 128);
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

	aoClose(aoDevice);
	
	return 0;
}

