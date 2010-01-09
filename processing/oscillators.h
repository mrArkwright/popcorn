#ifndef OSCILLATORS_H
#define OSCILLATORS_H

#include "misc.h"

typedef struct {
	char init;
	char act;
	float val;
	float phase;
	float (*func)(float, float, float);
	param freq, vol, param1;
} osc;


/* --- Initialization --- */

void initOsc(osc*, float(*)(float, float, float), float*, float*, float*);


/* --- Computing --- */

void compOsc(osc*);

float oscSin(float, float, float);
float oscTri(float, float, float);
float oscRec(float, float, float);


/* Control */

void startOsc(osc *o);
void stopOsc(osc *o);
void pauseOsc(osc *o);

#endif
