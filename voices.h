#include <math.h>
#include "oscillators.h"

#define voiceCount 16
#define oscCount 4
#define lfoCount 4

typedef struct {
	char act;
	float freq;
	int velocity;
	osc oscs[oscCount];
	osc lfos[lfoCount];
	float* output;
} voice;

void initVoices();
void initVoice(voice*);
void routeVoice(voice*);

void compVoices();
void compVoice(voice*);

void playSound(int note, int velocity);
void stopSound(int note, int velocity);

float getFreq(int note);

voice voices[voiceCount];
