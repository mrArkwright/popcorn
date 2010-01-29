#include "compute.h"

/* --- Units --- */
unit **gUnits = NULL; /* array with pointers to all global units */
int gUnitCount = 0; /* number of global units */

unit **lUnits = NULL; /* array with pointers to all local units */
int lUnitCount = 0; /* number of local units */

float voicesOutput; /* summing value of all voices */
float *masterOutput = NULL; /* pointer to global unit value for master output */

void compute() {
	int i, j;

	for (i = 0; i < gUnitCount; i++) { /* process all global units with specified computing function */
		if (*(gUnits[i]->acts[0]) == 1) {
			gUnits[i]->comp(gUnits[i]->units[0]);
		}
	}

	for (i = 0; i < lUnitCount; i++) { /* process all local units with specified computing function */
		for (j = 0; j < voiceCount; j++) { /* local units have an inner unit for every voice */
			if (*(lUnits[i]->acts[j]) == 1) {
				lUnits[i]->comp(lUnits[i]->units[j]);
			}
		}
	}

	voicesOutput = 0;
	for (i = 0; i < voiceCount; i++) { /* sum voice output values */
		voicesOutput += *(voices[i].output);
	}
}
