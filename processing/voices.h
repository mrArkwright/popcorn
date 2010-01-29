#ifndef PROCESSING_VOICES_H
#define PROCESSING_VOICES_H

#include "oscillators.h"
#include "tools.h"
#include "effects.h"

/*#define DEBUG_VOICES*/

extern int voiceCount;

typedef struct voice {
	char act; /* voice is playing */
	struct voice *preVoice, *postVoice; /* poiners to preceding and succeding voice for voice-steeling-algorithm */
	float freq;
	float velocity;
	float *output; /* pointer to an output value of a local unit for the voices-summing */
} voice;

extern voice *voices;
extern voice *firstVoice, *lastVoice;
extern int actVoices;


/* --- Setup --- */

void setupVoices();
void setupVoice(voice *);


/* Control */

void startVoice(int note, int velocity);
void stopVoice(int note, int velocity);


/* Debug */

#ifdef DEBUG_VOICES
void debugVoices();
#endif

#endif
