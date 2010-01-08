#ifndef EFFECTS_H
#define EFFECTS_H

#include "misc.h"

extern unsigned int sampleRate;

typedef struct {
	char init;
	char act;
	float val;
	
} fxBandpass;

float iirCutoff;

float fxEcho(float);
float fxFIR(float);
float fxIIR(float);
float fxIIRTest(float, float);

#endif
