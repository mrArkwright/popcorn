#ifndef MISC_H
#define MISC_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define defParamCount 4

extern float **gParams;
extern int gParamCount;
extern char gBools[];
extern float defParams[];

extern unsigned int sampleRate;

typedef struct {
	float *val;
	float *mod;
	float *range;
} param;


/* --- Setup --- */

void setupParam(param *);


/* --- Computing --- */

float compParam(param *);


/* Stuff */

float getFreq(int);

#endif
