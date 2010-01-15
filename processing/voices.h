#ifndef VOICES_H
#define VOICES_H

#include "oscillators.h"
#include "tools.h"
#include "effects.h"

#define voiceCount 32

typedef struct voice {
	char act;
	struct voice *preVoice, *postVoice;
	float freq;
	float velocity;
	float* output;
} voice;

extern voice voices[voiceCount];
extern voice *firstVoice, *lastVoice;
extern float actVoices;


/* --- Initialization --- */

void initVoices();
void initVoice(voice*);


/* Control */

void startVoice(int note, int velocity);
void stopVoice(int note, int velocity);

#endif
