#include "routing.h"

float *routeMasterOutput(gUnit *u) {
	switch(u->type) {
		case utOSC:
			masterOutput = &(((osc*)(u->unit))->val);
			break;
		default: 
			masterOutput = NULL;
			break;
	}

	return masterOutput;
}

void setGlobalStaticParam(gUnit *u, int param, float val) {
	switch(u->type) {
		
	}
}


void routeParam(gUnit *u, int param, gUnit *in) {
	
}

gUnit *addGlobalOsc(int type) {
	gUnit *newUnit = addGlobalUnit();
	float (*func)(float, float, float);

	newUnit->type = utOSC;
	newUnit->unit = malloc(sizeof(osc));
	newUnit->comp = &compOsc;

	switch(type) {
		case otSIN: func = &oscSin; break;
		case otTRI: func = &oscTri; break;
		case otREC: func = &oscRec; break;
		default: break;
	}

	initOsc(newUnit->unit, func, defParams+0, defParams+1, defParams+2);

	newUnit->act = gBools+1;

	return newUnit;
}

gUnit *addGlobalUnit() {
	gUnitCount++;
	gUnits = realloc(gUnits, sizeof(gUnit) * gUnitCount);
	return gUnits + gUnitCount - 1;
}














void taddOsc() {
	gUnit *newUnit;

	gUnitCount++;
	gUnits = realloc(gUnits, sizeof(gUnit) * gUnitCount);

	newUnit = gUnits + gUnitCount - 1;
	
	newUnit->unit = malloc(sizeof(osc));
	newUnit->comp = &compOsc;
	initOsc(newUnit->unit, &oscSin, defParams+0, defParams+1, defParams+2);
	newUnit->act = gBools+1;
}

void taddLOsc() {
	int i;
	lUnit *newUnit;

	lUnitCount++;
	lUnits = realloc(lUnits, sizeof(lUnit) * lUnitCount);

	newUnit = lUnits + lUnitCount - 1;

	newUnit->units = malloc(sizeof(void*) * voiceCount);
	newUnit->act = malloc(sizeof(char*) * voiceCount);
	newUnit->comp = &compOsc;

	for (i = 0; i < voiceCount; i++) {
		newUnit->units[i] = malloc(sizeof(osc));
		initOsc(newUnit->units[i], &oscSin, &(voices[i].freq), defParams+1, defParams+2);

		newUnit->act[i] = &(voices[i].act);

		voices[i].output = &(((osc*)(newUnit->units[i]))->val);
	}
}
