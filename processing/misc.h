#ifndef MISC_H
#define MISC_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/* TODO: use this pi */
#define twoPi (2 * 3.14159265358979323846)

#define defParamCount 4

extern float **gParams;
extern int gParamCount;
extern char gBools[];
extern float defParams[];

extern unsigned int sampleRate;

typedef struct { /* type used for all routable params. therefore values of params are always pointers */
	float *val; /* center-value for param */
	float *mod; /* "val" can be modulated by this value */
	float *range; /* the range by which "mod" modulates "val" */
} param;


/* --- Setup --- */

void setupParam(param *);


/* --- Computing --- */

float compParam(param *);


/* Stuff */

float getFreq(int);

#endif
