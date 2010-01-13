#include "routing.h"


/* --- Configuration API --- */

void routeMasterOutput(unit *u) {
	if (u->scope == usGLOBAL) {
		masterOutput = getValAddress(u, 0);
	}
}

void routeVoicesOutput(unit *u) {
	int i;
	
	if (u->scope == usLOCAL) {
		for (i = 0; i < voiceCount; i++) {
			voices[i].output = getValAddress(u, i);
		}
	}
}

void setParam(unit *u, int type, int option, float val) {
	float **p, **pLoc;
	int i;

	p = getParamAddress(u, type, option, 0);

	if (!isGlobalParam(p)) {
		*p = addGlobalParam();
	}

	if (u->scope == usLOCAL) {
		for (i = 1; i < voiceCount; i++) {
			pLoc = getParamAddress(u, type, option, i);
			*pLoc = *p;
		}
	}

	**p = val;
}

void routeParam(unit *u, int type, int option, unit *src) {
	float **p;
	float *v;
	int i;

	/* wenn gParam -> löschen*/

	if (u->scope == usGLOBAL) {
		if (src->scope == usGLOBAL) {
			p = getParamAddress(u, type, option, 0);
			v = getValAddress(src, 0);

			*p = v;
		} /* else fehler */
	} else {
		for (i = 0; i < voiceCount; i++) {
			p = getParamAddress(u, type, option, i);
			v = getValAddress(src, ((src->scope == usGLOBAL) ? 0 : i));

			*p = v;
		}
	}
}

void setBool(unit *u, int type, char val) {
	char **b, *bSrc;
	int i;

	if (val == 0) {
		bSrc = gBools + 0;
	} else {
		bSrc = gBools + 1;
	}

	b = getBoolAddress(u, type, 0);
	*b = bSrc;

	if (u->scope == usLOCAL) {
		for (i = 1; i < voiceCount; i++) {
			b = getBoolAddress(u, type, i);
			*b = bSrc;
		}
	}
}

/*void routeGlobalBool(unit *u, int type, unit *src) {
	char *p = (char *) getGlobalValAddress(src);

	switch (type) {
		case btACT: u->act = p;
		default: break;
	}
}*/


/* - Oscillators */

/*unit *addGlobalOsc(int type) {
	gUnit *newUnit = addGlobalUnit();

	newUnit->type = utOSC;
	newUnit->unit = malloc(sizeof(osc));
	newUnit->comp = (void (*)(void *))&compOsc;

	setupOsc(newUnit->unit, getOscFunc(type));

	newUnit->act = gBools + 1;

	return newUnit;
}

unit *addLocalOsc(int type) {
	int i;

	lUnit *newUnit = addLocalUnit();

	newUnit->type = utOSC;
	newUnit->units = malloc(sizeof(osc *) * voiceCount);
	newUnit->acts = malloc(sizeof(char *) * voiceCount);
	newUnit->comp = (void (*)(void *))&compOsc;

	for (i = 0; i < voiceCount; i++) {
		newUnit->units[i] = malloc(sizeof(osc));
		setupOsc(newUnit->units[i], getOscFunc(type));

		newUnit->acts[i] = gBools + 1;
	}

	return newUnit;
}

float (*getOscFunc(int type))(float, float, float) {*/
	/* Oscillator Types */
	/*switch (type) {
		case otSIN: return &oscSin;
		case otTRI: return &oscTri;
		case otREC: return &oscRec;
		default: return NULL;
	}
}*/







/* ---Helper ---*/

float *addGlobalParam() {
	gParamCount++;
	gParams = realloc(gParams, sizeof(float *) * gParamCount);
	gParams[gParamCount - 1] = malloc(sizeof(float));
	return gParams[gParamCount - 1];
}

float **getParamAddress(unit *u, int type, int option, int i) {
	param *p;
	
	/* Unit-Type-Params */
	switch (u->type) {
		case utOSC:
			switch (type) {
				case ptFREQ: p = &(((osc*)(u->units[i]))->freq); break;
				case ptVOL: p = &(((osc*)(u->units[i]))->vol); break;
				case ptPARAM1: p = &(((osc*)(u->units[i]))->param1); break;
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

char **getBoolAddress(unit *u, int type, int i) {
	switch (type) {
		case btACT: return &(u->acts[i]);
		default: break;
	}

	/* Unit-Type-Bools */

	return NULL;
}

char isGlobalParam(float **p) {
	int i;

	for (i = 0; i < gParamCount; i++) {
		if (gParams[i] == *p) return 1;
	}

	return 0;
}

float *getValAddress(unit *u, int i) {

	/* Unit-Type-Vals */
	switch (u->type) {
		case utOSC: return &(((osc*)(u->units[i]))->val);
		default: return NULL;
	}
}










/*unit *addGlobalUnit() {
	gUnitCount++;
	gUnits = realloc(gUnits, sizeof(gUnit *) * gUnitCount);
	gUnits[gUnitCount - 1] = malloc(sizeof(gUnit));
	return gUnits[gUnitCount - 1];
}*/


/*unit *addLocalUnit() {
	lUnitCount++;
	lUnits = realloc(lUnits, sizeof(unit *) * lUnitCount);
	lUnits[lUnitCount - 1] = malloc(sizeof(lUnit));
	return lUnits[lUnitCount - 1];
}*/
