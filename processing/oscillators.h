#ifndef PROCESSING_OSCILLATORS_H
#define PROCESSING_OSCILLATORS_H

#include "misc.h"

typedef struct { /* type for oscillator units TODO: phase shifting, reinit by restart */
	float val; /* the current value of a the osc */
	float phase;
	float (*func)(float, float, float); /* specific compute function (oscSin etc) */
	param freq, vol, param1, phaseShift; /* the freq, volume, pulse modulation (or s.th. similar) and phaseShift */
} osc;


/* --- Setup --- */

void setupOsc(osc *);


/* --- Reinit --- */

void reinitOsc(osc *);


/* --- Computing --- */

void compOsc(osc *);

float oscSin(float, float, float);
float oscTri(float, float, float);
float oscRec(float, float, float);

#endif
