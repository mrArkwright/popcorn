/*
 *  main program
 *
 *  for now:
 *  - init audio
 *  - fill buffer
 */

#include <stdio.h>
#include <SDL.h>

#include "oscillators.h"
#include "audioout.h"

int main(void) {
	audioInfo aInfo;
	/* UI Foo */
	SDL_Surface *screen;
	SDL_Event event;
	int running;
	screen = SDL_SetVideoMode(200,200, 16, SDL_SWSURFACE);
	SDL_WM_SetCaption("Audio Example",0);
	/* UI Foo End */

	
	/* passing callback to sdlInit() */
	sdlinit(&example_mixaudio, &aInfo);

	sampleRate = aInfo.sampleRate;

	/* seting up routing */
	oscRoute();

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

