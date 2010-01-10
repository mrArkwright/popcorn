#include "compute.h"

/* --- Units --- */
gUnit **gUnits = NULL;
int gUnitCount = 0;

lUnit **lUnits = NULL;
int lUnitCount = 0;

float voicesOutput;
float *masterOutput = NULL;

void compute() {
	int i, j;

	for (i = 0; i < gUnitCount; i++) {
		if (*(gUnits[i]->act) == 1) {
			gUnits[i]->comp(gUnits[i]->unit);
		}
	}

	for (i = 0; i < lUnitCount; i++) {
		for (j = 0; j < voiceCount; j++) {
			if (*(lUnits[i]->act[j]) == 1) {
				lUnits[i]->comp(lUnits[i]->units[j]);
			}
		}
	}

	voicesOutput = 0;
	for (i = 0; i < voiceCount; i++) {
		/*voicesOutput += *(voices[i].output);*/
	}
}
