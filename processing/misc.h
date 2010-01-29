#ifndef PROCESSING_MISC_H
#define PROCESSING_MISC_H

#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define twoPi (2.0 * 3.14159265358979323846)

#define defValueCount 4

extern float **gValues;
extern int gValueCount;
extern char gBools[];
extern float defValues[];

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
