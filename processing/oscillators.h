#ifndef OSCILLATORS_H
#define OSCILLATORS_H

#include "misc.h"

unsigned int sampleRate;

typedef struct {
	char init;
	char act;
	float val;
	float phase;
	float (*func)(float, float, float);
	param freq, vol, param1;
} osc;

void initOsc(osc*, float(*)(float, float, float), float*, float*, float*);

void startOsc(osc *o);
void stopOsc(osc *o);
void pauseOsc(osc *o);

void compOsc(osc*);

float oscSin(float, float, float);
float oscTri(float, float, float);
float oscRec(float, float, float);

#endif
