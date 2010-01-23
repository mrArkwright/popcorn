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


/* --- Setup --- */

void setupFxLowpass(fxLowpass *);
void setupFxHighpass(fxHighpass *);


/* --- Computing --- */

void compFxLowpass(fxLowpass *);
void compFxHighpass(fxHighpass *);






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
