#ifndef VOICES_H
#define VOICES_H

#include "oscillators.h"
#include "tools.h"
#include "effects.h"

extern int voiceCount;

typedef struct voice {
	char act;
	struct voice *preVoice, *postVoice;
	float freq;
	float velocity;
	float *output;
} voice;

extern voice *voices;
extern voice *firstVoice, *lastVoice;
extern float actVoices;


/* --- Setup --- */

void setupVoices();
void setupVoice(voice *);


/* Control */

void startVoice(int note, int velocity);
void stopVoice(int note, int velocity);

#endif
