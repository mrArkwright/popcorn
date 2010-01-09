/*
 *  main program
 *
 *  for now:
 *  - init audio
 *  - fill buffer
 */

#include <stdio.h>
#include <SDL.h>

/*#include "oscillators.h"*/
#include "mixAudio.h"
#include "audioout.h"

#include "configuration/routing.h"

unsigned int sampleRate;

int main(int argc, char *argv[]) {
	audioInfo aInfo;
	/* UI Foo */
	SDL_Surface *screen;
	SDL_Event event;
	int running;
	screen = SDL_SetVideoMode(200,200, 16, SDL_SWSURFACE);
	SDL_WM_SetCaption("Audio Example",0);
	/* UI Foo End */

	/* setting up voices */
	route();

	/* passing callback to sdlInit() */
	sdlinit(&mixAudio, &aInfo);

	sampleRate = aInfo.sampleRate;

	/* More UI Foo */
	running = 1;
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
	
	return 0;
}

