#ifndef PROCESSING_COMPUTE_H
#define PROCESSING_COMPUTE_H

#include "oscillators.h"
#include "tools.h"
#include "effects.h"
#include "voices.h"

typedef struct { /* type for all local and global units */
	char scope; /* is unit local (usLOCAL) or global (usGLOBAL), specified with enum unitScope */
	char type; /* type of the unit is one of enum unitType (e.g. utOSC) */
	char **acts; /* inner unit will only be computed, if this points to a value unequal 0 */
	void **units; /* inner units. global units have exactly one, local units have an inner unit for every voice */
	void (*comp)(void *); /* pointer to computing function of the unit type */
	void (*reinit)(void *); /* pointer to reinit function of the unit type */
} unit;

extern unit **gUnits;
extern int gUnitCount;

extern unit **lUnits;
extern int lUnitCount;

extern float voicesOutput;
extern float *masterOutput;

void compute();

#endif
