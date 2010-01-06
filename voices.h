#include <math.h>
#include "oscillators.h"
#include "effects.h"

#define voiceCount 128
#define oscCount 4
#define lfoCount 4
#define globLfoCount 4
#define globParamCount 32

typedef struct {
	char act;
	float freq;
	int velocity;
	osc oscs[oscCount];
	osc lfos[lfoCount];
	float* output;
} voice;

void initVoices();
void initVoice(voice*, int);
void routeVoice(voice*);

float compVoices();
void compVoice(voice*);

void playSound(int note, int velocity);
void stopSound(int note, int velocity);

float getFreq(int note);

voice voices[voiceCount];
osc gLfos[globLfoCount];
osc lfo1;
float gParams[globParamCount];
