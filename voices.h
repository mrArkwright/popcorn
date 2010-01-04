#include "oscillators.h"

#define voiceCount 16
#define oscCount 4
#define lfoCount 4

typedef struct {
	char act;
	int note;
	int velocity;
	osc oscs[oscCount];
	osc lfos[lfoCount];
	float* output;
} voice;

void initVoices();
void initVoice(voice*);
void routeVoice(voice*);

void compVoices();

void playSound(int note, int velocity);
void stopSound(int note, int velocity);

voice voices[voiceCount];
