#ifndef COMPUTE_H
#define COMPUTE_H

#include "oscillators.h"
#include "tools.h"
#include "effects.h"
#include "voices.h"

typedef struct {
	char type;
	char *act;
	void *unit;
	void (*comp)(void*);
} gUnit;

typedef struct {
	char type;
	char **act;
	void **units;
	void (*comp)(void*);
} lUnit;

extern gUnit *gUnits;
extern int gUnitCount;

extern lUnit *lUnits;
extern int lUnitCount;

extern float voicesOutput;
extern float *masterOutput;

void compute();

#endif
