#ifndef OSCILLATORS_H
#define OSCILLATORS_H

#include "misc.h"

typedef struct {
	float val;
	float phase;
	float (*func)(float, float, float);
	param freq, vol, param1;
} osc;


/* --- Setup --- */

void setupOsc(osc *o, float(*)(float, float, float));


/* --- Computing --- */

void compOsc(osc*);

float oscSin(float, float, float);
float oscTri(float, float, float);
float oscRec(float, float, float);

#endif
