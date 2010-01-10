#include "routing.h"


/* --- API ---*/

float *routeMasterOutput(gUnit *u) {
	masterOutput = getGlobalValAddress(u);
	return masterOutput;
}

gUnit *addGlobalOsc(int type) {
	gUnit *newUnit = addGlobalUnit();
	float (*func)(float, float, float);

	newUnit->type = utOSC;
	newUnit->unit = malloc(sizeof(osc));
	newUnit->comp = &compOsc;

	/* Oscillator-Types */
	switch (type) {
		case otSIN: func = &oscSin; break;
		case otTRI: func = &oscTri; break;
		case otREC: func = &oscRec; break;
		default: break;
	}

	setupOsc(newUnit->unit, func);

	newUnit->act = gBools+1;

	return newUnit;
}

void setGlobalParam(gUnit *u, int type, int option, float val) {
	float **p = getGlobalParamAddress(u, type, option);

	if (isParamDefault(p) || *p == NULL) {
		*p = addGlobalParam();
	}

	**p = val;
}

void routeGlobalParam(gUnit *u, int type, int option, gUnit *src) {
	float **p = getGlobalParamAddress(u, type, option);
	float *v = getGlobalValAddress(src);

	/* wenn gParam -> löschen*/
	*p = v;
}


/* ---Helper ---*/

gUnit *addGlobalUnit() {
	gUnitCount++;
	gUnits = realloc(gUnits, sizeof(gUnit *) * gUnitCount);
	gUnits[gUnitCount - 1] = malloc(sizeof(gUnit));
	return gUnits[gUnitCount - 1];
}

float *addGlobalParam() {
	gParamCount++;
	gParams = realloc(gParams, sizeof(float *) * gParamCount);
	gParams[gParamCount - 1] = malloc(sizeof(float));
	return gParams[gParamCount - 1];
}

float **getGlobalParamAddress(gUnit *u, int type, int option) {
	param *p;
	
	/* Unit-Types */
	switch (u->type) {
		case utOSC:
			switch (type) {
				case ptFREQ: p = &(((osc*)(u->unit))->freq); break;
				case ptVOL: p = &(((osc*)(u->unit))->vol); break;
				case ptPARAM1: p = &(((osc*)(u->unit))->param1); break;
				default: return NULL;
			}
			break;
		default: return NULL;
	}

	switch (option) {
		case poVAL: return &(p->val);
		case poMOD: return &(p->mod);
		case poRANGE: return &(p->range);
		default: return NULL;
	}
}

float *getGlobalValAddress(gUnit *u) {

	/* Unit-Types */
	switch (u->type) {
		case utOSC: return &(((osc*)(u->unit))->val);
		default: return NULL;
	}
}

char isParamDefault(float **p) {
	int i;

	for (i = 0; i < defParamCount; i++) {
		if ((defParams + i) == *p) return 1;
	}

	return 0;
}
