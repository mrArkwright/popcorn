#include "routing.h"

unit voiceDummies[4];
unit *voiceActive = voiceDummies, *voiceFreq = voiceDummies + 1, *voiceVelocity = voiceDummies + 2, *voicesOut = voiceDummies + 3;


/* --- Configuration API --- */

/* - common */
void setVoiceCount(int count) {
	voiceCount = count;

	setupVoices();
}

void setupRouting() {
	masterOutput = defParams + 0;

	voiceActive->scope = usLOCAL;
	voiceActive->type = utVOICE_ACT;

	voiceFreq->scope = usLOCAL;
	voiceFreq->type = utVOICE_FREQ;

	voiceVelocity->scope = usLOCAL;
	voiceVelocity->type = utVOICE_VEL;

	voicesOut->scope = usGLOBAL;
	voicesOut->type = utVOICE_OUTPUT;
}

void routeMasterOutput(unit *src) {
	if (src->scope == usGLOBAL) {
		masterOutput = getValAddress(src, 0);
	}
}

void routeVoicesOutput(unit *src) {
	int i;
	
	if (src->scope == usLOCAL) {
		for (i = 0; i < voiceCount; i++) {
			voices[i].output = getValAddress(src, i);
		}
	}
}

/* - Params */
void setParam(unit *u, paramType type, paramOption option, float val) {
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

void routeParam(unit *u, paramType type, paramOption option, unit *src) {
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
void setBool(unit *u, boolType type, char val) {
	char **b, *bSrc;
	int i, iMax = ((u->scope == usGLOBAL) ? 1 : voiceCount);

	if (val == 0) {
		bSrc = gBools + 0;
	} else {
		bSrc = gBools + 1;
	}

	for (i = 0; i < iMax; i++) {
		b = getBoolParamAddress(u, type, i);
		*b = bSrc;
	}
}

void routeBool(unit *u, boolType type, unit *src) {
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
unit *addOsc(unitScope scope) {
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

void setOscType(unit *u, oscType type) {
	int i, iMax = ((u->scope == usGLOBAL) ? 1 : voiceCount);

	for (i = 0; i < iMax; i++) {
		((osc *)(u->units[i]))->func = getOscFunc(type);
	}
}


/* - Mixers*/
unit *addMixer2ch(unitScope scope) {
	unit *newUnit = addUnit(scope);
	int i, iMax = ((scope == usGLOBAL) ? 1 : voiceCount);

	newUnit->type = utMIXER2CH;
	newUnit->comp = (void (*)(void *))&compMixer2ch;

	for (i = 0; i < iMax; i++) {
		newUnit->units[i] = malloc(sizeof(mixer2ch));
		setupMixer2ch(newUnit->units[i]);
	}

	return newUnit;
}


/* - Lowpass-Filter */
unit *addFxLowpass(unitScope scope) {
	unit *newUnit = addUnit(scope);
	int i, iMax = ((scope == usGLOBAL) ? 1 : voiceCount);

	newUnit->type = utFX_LOWPASS;
	newUnit->comp = (void (*)(void *))&compFxLowpass;

	for (i = 0; i < iMax; i++) {
		newUnit->units[i] = malloc(sizeof(fxLowpass));
		setupFxLowpass(newUnit->units[i]);
	}

	return newUnit;
}


/* - Highpass-Filter */
unit *addFxHighpass(unitScope scope) {
	unit *newUnit = addUnit(scope);
	int i, iMax = ((scope == usGLOBAL) ? 1 : voiceCount);

	newUnit->type = utFX_HIGHPASS;
	newUnit->comp = (void (*)(void *))&compFxHighpass;

	for (i = 0; i < iMax; i++) {
		newUnit->units[i] = malloc(sizeof(fxHighpass));
		setupFxHighpass(newUnit->units[i]);
	}

	return newUnit;
}

/* - Bandpass-Filter */
unit *addFxBandpass(unitScope scope) {
	unit *newUnit = addUnit(scope);
	int i, iMax = ((scope == usGLOBAL) ? 1 : voiceCount);

	newUnit->type = utFX_BANDPASS;
	newUnit->comp = (void (*)(void *))&compFxBandpass;

	for (i = 0; i < iMax; i++) {
		newUnit->units[i] = malloc(sizeof(fxBandpass));
		setupFxBandpass(newUnit->units[i]);
	}

	return newUnit;
}



/* ---Helpers ---*/

/* - common */
unit *addUnit(unitScope scope) {
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

float **getParamAddress(unit *u, paramType type, paramOption option, int i) {
	param *p;
	
	/* Unit-Type-Params */
	switch (u->type) {
		case utOSC:
			switch (type) {
				case ptFREQ: p = &(((osc *)(u->units[i]))->freq); break;
				case ptVOL: p = &(((osc *)(u->units[i]))->vol); break;
				case ptPARAM1: p = &(((osc *)(u->units[i]))->param1); break;
				default: return NULL;
			}
			break;
		case utMIXER2CH:
			switch (type) {
				case ptINPUT1: p = &(((mixer2ch *)(u->units[i]))->input1); break;
				case ptINPUT2: p = &(((mixer2ch *)(u->units[i]))->input2); break;
				case ptVOL1: p = &(((mixer2ch *)(u->units[i]))->vol1); break;
				case ptVOL2: p = &(((mixer2ch *)(u->units[i]))->vol2); break;
				default: return NULL;
			}
			break;
		case utFX_LOWPASS:
			switch (type) {
				case ptINPUT1: p = &(((fxLowpass *)(u->units[i]))->input); break;
				case ptCUTOFF: p = &(((fxLowpass *)(u->units[i]))->cutoff); break;
				default: return NULL;
			}
			break;
		case utFX_HIGHPASS:
			switch (type) {
				case ptINPUT1: p = &(((fxHighpass *)(u->units[i]))->input); break;
				case ptCUTOFF: p = &(((fxHighpass *)(u->units[i]))->cutoff); break;
				default: return NULL;
			}
			break;
		case utFX_BANDPASS:
			switch (type) {
				case ptINPUT1: p = &(((fxBandpass *)(u->units[i]))->input); break;
				case ptFREQ: p = &(((fxBandpass *)(u->units[i]))->freq); break;
				case ptBANDWIDTH: p = &(((fxBandpass *)(u->units[i]))->bandwidth); break;
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
		case utVOICE_FREQ: return &(voices[i].freq);
		case utVOICE_VEL: return &(voices[i].velocity);
		case utVOICE_OUTPUT: return &voicesOutput;
		case utOSC: return &(((osc *)(u->units[i]))->val);
		case utMIXER2CH: return &(((mixer2ch *)(u->units[i]))->val);
		case utFX_LOWPASS: return &(((fxLowpass *)(u->units[i]))->val);
		case utFX_HIGHPASS: return &(((fxHighpass *)(u->units[i]))->val);
		case utFX_BANDPASS: return &(((fxBandpass *)(u->units[i]))->val);
		default: return NULL;
	}
}

/* - Bools */
char **getBoolParamAddress(unit *u, boolType type, int i) {
	switch (type) {
		case btACT: return &(u->acts[i]);
		default: break;
	}

	/* Unit-Type-Bool-Params */
	switch (u->type) {
		case utMIXER2CH:
			switch (type) {
				case btACT1: return &(((mixer2ch *)(u->units[i]))->act1);
				case btACT2: return &(((mixer2ch *)(u->units[i]))->act2);
				default: return NULL;
			}
			break;
		default: return NULL;
	}
}

char *getBoolValAddress(unit *u, int i) {
	
	/* Unit-Type-Bool-Vals*/
	switch (u->type) {
		case utVOICE_ACT: return &(voices[i].act);
		default: return NULL;
	}
}

/* - Oscillators */
float (*getOscFunc(oscType type))(float, float, float) {

	/* Oscillator Types */
	switch (type) {
		case otSIN: return &oscSin;
		case otTRI: return &oscTri;
		case otREC: return &oscRec;
		default: return NULL;
	}
}
