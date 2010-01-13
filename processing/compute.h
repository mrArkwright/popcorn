#ifndef COMPUTE_H
#define COMPUTE_H

#include "oscillators.h"
#include "tools.h"
#include "effects.h"
#include "voices.h"

typedef struct {
	char scope;
	char type;
	char **acts;
	void **units;
	void (*comp)(void *);
} unit;

extern unit **gUnits;
extern int gUnitCount;

extern unit **lUnits;
extern int lUnitCount;

extern float voicesOutput;
extern float *masterOutput;

void compute();

#endif
