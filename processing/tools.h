#ifndef TOOLS_H
#define TOOLS_H

#include "misc.h"

typedef struct {
	char act;
	float *input;
	param vol;
} bus;

typedef struct {
	float val;
	int busCount;
	bus *busses;
} mixer;


/* --- Initialization --- */

void initMixer(mixer*);


/* --- Computing --- */

void compMixer(mixer*);

#endif
