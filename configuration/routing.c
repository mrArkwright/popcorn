#include "routing.h"



/* --- Configuration API --- */

/* - common */
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

/* - Params */
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
	float **p, *pSrc;
	int i;

	/* wenn gParam -> löschen*/

	if (u->scope == usGLOBAL) {
		if (src->scope == usGLOBAL) {
			p = getParamAddress(u, type, option, 0); /* unit gibt zB bool zurück wenn ergebniss == NULL -> fehler */
			pSrc = getValAddress(src, 0);

			*p = pSrc;
		} /* else fehler */
	} else {
		for (i = 0; i < voiceCount; i++) {
			p = getParamAddress(u, type, option, i);
			pSrc = getValAddress(src, ((src->scope == usGLOBAL) ? 0 : i));

			*p = pSrc;
		}
	}
}

/* - Bools */
void setBool(unit *u, int type, char val) {
	char **b, *bSrc;
	int i, iMax = ((u->scope == usGLOBAL) ? 1 : voiceCount);

	if (val == 0) {
		bSrc = gBools + 0;
	} else {
		bSrc = gBools + 1;
	}

	for (i = 1; i < iMax; i++) {
		b = getBoolParamAddress(u, type, i);
		*b = bSrc;
	}
}

void routeBool(unit *u, int type, unit *src) {
	char **b, *bSrc;
	int i;

	if (u->scope == usGLOBAL) {
		if (src->scope == usGLOBAL) {
			b = getBoolParamAddress(u, type, 0);
			bSrc = getBoolValAddress(src, 0);

			*b = bSrc;
		} /* else fehler */
	} else {
		for (i = 0; i < voiceCount; i++) {
			b = getBoolParamAddress(u, type, i);
			bSrc = getBoolValAddress(src, ((src->scope == usGLOBAL) ? 0 : i));

			*b = bSrc;
		}
	}
}


/* - Oscillators */
unit *addOsc(int scope) {
	unit *newUnit = addUnit(scope);
	int i, iMax = ((scope == usGLOBAL) ? 1 : voiceCount);

	newUnit->type = utOSC;
	newUnit->comp = (void (*)(void *))&compOsc;

	for (i = 0; i < iMax; i++) {
		newUnit->units[i] = malloc(sizeof(osc));
		setupOsc(newUnit->units[i]);
	}

	return newUnit;
}



/* ---Helper ---*/

/* - common */
unit *addUnit(int scope) {
	unit *newUnit;
	int i;

	if (scope == usGLOBAL) {
		gUnitCount++;
		gUnits = realloc(gUnits, sizeof(unit *) * gUnitCount);
		newUnit = gUnits[gUnitCount - 1] = malloc(sizeof(unit));

		newUnit->units = malloc(sizeof(void *));
		newUnit->acts = malloc(sizeof(char *));
		newUnit->acts[0] = gBools + 1;
	} else {
		lUnitCount++;
		lUnits = realloc(lUnits, sizeof(unit *) * lUnitCount);
		newUnit = lUnits[lUnitCount - 1] = malloc(sizeof(unit));

		newUnit->units = malloc(sizeof(void *) * voiceCount);
		newUnit->acts = malloc(sizeof(char *) * voiceCount);
		for (i = 0; i < voiceCount; i++) {
			newUnit->acts[i] = gBools + 1;
		}
	}

	newUnit->scope = scope;

	return newUnit;
}

/* - Params */
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

/* - Bools */
char **getBoolParamAddress(unit *u, int type, int i) {
	switch (type) {
		case btACT: return &(u->acts[i]);
		default: break;
	}

	/* Unit-Type-Bool-Params */

	return NULL;
}

char *getBoolValAddress(unit *u, int i) {
	
	/* Unit-Type-Bool-Vals*/
	switch (u->type) {
		case utVOICE: return NULL; /* <- !!!!!!!!!!!!!!!!!!! */
		default: return NULL;
	}
}

/* - Oscillators */
float (*getOscFunc(int type))(float, float, float) {

	/* Oscillator Types */
	switch (type) {
		case otSIN: return &oscSin;
		case otTRI: return &oscTri;
		case otREC: return &oscRec;
		default: return NULL;
	}
}
