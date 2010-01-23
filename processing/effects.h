#ifndef EFFECTS_H
#define EFFECTS_H

#include "misc.h"

typedef struct {
	float val;
	param cutoff, input;
} fxLowpass;

typedef struct {
	float val;
	float tmp;
	param cutoff, input;
} fxHighpass;

typedef struct {
	float val;
	float tmp;
	param freq, bandwidth, input;
} fxBandpass;


/* --- Setup --- */

void setupFxLowpass(fxLowpass *);
void setupFxHighpass(fxHighpass *);
void setupFxBandpass(fxBandpass *);


/* --- Computing --- */

void compFxLowpass(fxLowpass *);
void compFxHighpass(fxHighpass *);
void compFxBandpass(fxBandpass *);






/*typedef struct {
	char init;
	char act;
	float val;
	
} fxBandpass;

float iirCutoff;

float fxEcho(float);
float fxFIR(float);
float fxIIR(float);
float fxIIRTest(float, float);*/

#endif
