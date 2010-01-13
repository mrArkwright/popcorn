#include "compute.h"

/* --- Units --- */
unit **gUnits = NULL;
int gUnitCount = 0;

unit **lUnits = NULL;
int lUnitCount = 0;

float voicesOutput;
float *masterOutput = NULL;

void compute() {
	int i, j;

	for (i = 0; i < gUnitCount; i++) {
		if (*(gUnits[i]->acts[0]) == 1) {
			gUnits[i]->comp(gUnits[i]->units[0]);
		}
	}

	for (i = 0; i < lUnitCount; i++) {
		for (j = 0; j < voiceCount; j++) {
			if (*(lUnits[i]->acts[j]) == 1) {
				lUnits[i]->comp(lUnits[i]->units[j]);
			}
		}
	}

	voicesOutput = 0;
	for (i = 0; i < voiceCount; i++) {
		voicesOutput += *(voices[i].output);
	}
}
