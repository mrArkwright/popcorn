#ifndef MISC_H
#define MISC_H

#include <stdlib.h>
#include <math.h>

extern unsigned int sampleRate;
extern float m2pi;

typedef struct {
	float *val;
	float *mod;
	float *range;
} param;


/* --- Initialization --- */

void initParam(param*, float*, float*);


/* --- Computing --- */

float compParam(param*);


/* Stuff */

float getFreq(int);

#endif
