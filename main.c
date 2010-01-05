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
	initVoices();

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
							playSound(69, 128);
							break;
						case SDLK_s:
							playSound(70, 128);
							break;
						case SDLK_d:
							playSound(71, 128);
							break;
						case SDLK_f:
							playSound(72, 128);
							break;
						case SDLK_g:
							playSound(73, 128);
							break;
						case SDLK_h:
							playSound(74, 128);
							break;
						case SDLK_j:
							playSound(75, 128);
							break;
						default:
							break;
					}
					break;
				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
						case SDLK_a:
							stopSound(69, 128);
							break;
						case SDLK_s:
							stopSound(70, 128);
							break;
						case SDLK_d:
							stopSound(71, 128);
							break;
						case SDLK_f:
							stopSound(72, 128);
							break;
						case SDLK_g:
							stopSound(73, 128);
							break;
						case SDLK_h:
							stopSound(74, 128);
							break;
						case SDLK_j:
							stopSound(75, 128);
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

