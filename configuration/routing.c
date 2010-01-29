#include "routing.h"

unit voiceDummies[4]; /* dummy units for voice routing. they have no real function and no params. you can only route the output to other units */
unit *voiceActive = voiceDummies, *voiceFreq = voiceDummies + 1, *voiceVelocity = voiceDummies + 2, *voicesOut = voiceDummies + 3;


/* --- Configuration API --- */

/* - common */
void setVoiceCount(int count) { /* set up specified number of voices. always call this before routing! */
	voiceCount = count;

	setupVoices(); /* TODO: cleanup existing voices */
}

void setupRouting() { /* set up some important variables. always call this before routing! */
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

void routeMasterOutput(unit *src) { /* routes maste output to output of a unit */
	if (src->scope == usGLOBAL) { /* master output can't be routed to a local unit */
		masterOutput = getValAddress(src, 0);
	} /* TODO: error return? */
}

void routeVoicesOutput(unit *src) { /* routes voicesOutput to output of a unit */
	int i;
	
	if (src->scope == usLOCAL) { /* voices output can't be routed to a global unit */
		for (i = 0; i < voiceCount; i++) {
			voices[i].output = getValAddress(src, i);
		}
	} /* TODO: error return? */
}

/* - Params */
void setParam(unit *u, paramType type, paramOption option, float val) { /* sets value of a param to static value */
	float **p, **pLoc;
	int i;

	p = getParamAddress(u, type, option, 0); /* get pointer to the value. if unit is global there is only one, if local we only need the first one here */

	if (!isGlobalParam(p)) { /* create a new static value, if value is no */
		*p = addGlobalParam();
	}

	if (u->scope == usLOCAL) { /* if unit is local, set all values of inner units to the same value */
		for (i = 1; i < voiceCount; i++) {
			pLoc = getParamAddress(u, type, option, i); /* get pointer to the value to set */
			*pLoc = *p;
		}
	}

	**p = val; /* cahnge content of the value to specified value */
}

void routeParam(unit *u, paramType type, paramOption option, unit *src) { /* routes value of a param to another unit */
	float **p, *pSrc;
	int i;

	/* TODO: delete gParam if it was one before */

	if (u->scope == usGLOBAL) { /* unit to route is global -> src-unit has to be global too */
		if (src->scope == usGLOBAL) {
			p = getParamAddress(u, type, option, 0); /* get pointer to the value to route TODO: error if value doesn't exist */
			pSrc = getValAddress(src, 0); /* get output of src-unit */

			*p = pSrc; /* route! */
		} /* error if src-unit is local */
	} else {
		for (i = 0; i < voiceCount; i++) { /* route value for every inner unit of local unit */
			p = getParamAddress(u, type, option, i);
			pSrc = getValAddress(src, ((src->scope == usGLOBAL) ? 0 : i)); /* if src is global, all values are routed to the same value */

			*p = pSrc;
		}
	}
}

/* - Bools */
void setBool(unit *u, boolType type, char val) { /* sets bool value to static value */
	char **b, *bSrc;
	int i, iMax = ((u->scope == usGLOBAL) ? 1 : voiceCount);

	if (val == 0) { /* bools can only have two specific values */
		bSrc = gBools + 0;
	} else {
		bSrc = gBools + 1;
	}

	for (i = 0; i < iMax; i++) {
		b = getBoolParamAddress(u, type, i); /* get pointer to the value to set */
		*b = bSrc;
	}
}

void routeBool(unit *u, boolType type, unit *src) { /* routes bool value to another unit */
	char **b, *bSrc;
	int i;

	if (u->scope == usGLOBAL) { /* unit to route is global -> src->unit has to be global too */
		if (src->scope == usGLOBAL) {
			b = getBoolParamAddress(u, type, 0); /* get pointer to the value to route TODO: errorhandling */
			bSrc = getBoolValAddress(src, 0);

			*b = bSrc;
		} /* TODO: errorhandling */
	} else {
		for (i = 0; i < voiceCount; i++) { /* route value for every inner unit of local unit */
			b = getBoolParamAddress(u, type, i);
			bSrc = getBoolValAddress(src, ((src->scope == usGLOBAL) ? 0 : i));

			*b = bSrc;
		}
	}
}


/* - Oscillators */
unit *addOsc(unitScope scope) { /* add oscillator unit */
	unit *newUnit = addUnit(scope); /* adding and setting up basics for new unit*/
	int i, iMax = ((scope == usGLOBAL) ? 1 : voiceCount);

	newUnit->type = utOSC;
	newUnit->comp = (void (*)(void *))&compOsc; /* setup computing function for unit */

	for (i = 0; i < iMax; i++) { /* setup one inner unit for global unit or one for every voice for local unit */
		newUnit->units[i] = malloc(sizeof(osc)); /* allocate memory for every inner unit */
		setupOsc(newUnit->units[i]); /* specific setup for oscillator */
	}

	return newUnit;
}

void setOscType(unit *u, oscType type) { /* set the type of an oscillator (sine, rectangle etc) */
	int i, iMax = ((u->scope == usGLOBAL) ? 1 : voiceCount);

	for (i = 0; i < iMax; i++) { /* for every inner unit */
		((osc *)(u->units[i]))->func = getOscFunc(type);
	}
}


/* - Mixers*/
unit *addMixer2ch(unitScope scope) { /* add 2 channel mixer unit */
	unit *newUnit = addUnit(scope); /* adding and setting up basics for new unit*/
	int i, iMax = ((scope == usGLOBAL) ? 1 : voiceCount);

	newUnit->type = utMIXER2CH;
	newUnit->comp = (void (*)(void *))&compMixer2ch; /* setup computing function for unit */

	for (i = 0; i < iMax; i++) { /* setup one inner unit for global unit or one for every voice for local unit */
		newUnit->units[i] = malloc(sizeof(mixer2ch)); /* allocate memory for every inner unit */
		setupMixer2ch(newUnit->units[i]); /* specific setup for 2 channel mixer */
	}

	return newUnit;
}


/* - Lowpass-Filter */
unit *addFxLowpass(unitScope scope) {
	unit *newUnit = addUnit(scope); /* adding and setting up basics for new unit*/
	int i, iMax = ((scope == usGLOBAL) ? 1 : voiceCount);

	newUnit->type = utFX_LOWPASS;
	newUnit->comp = (void (*)(void *))&compFxLowpass; /* setup computing function for unit */

	for (i = 0; i < iMax; i++) { /* setup one inner unit for global unit or one for every voice for local unit */
		newUnit->units[i] = malloc(sizeof(fxLowpass)); /* allocate memory for every inner unit */
		setupFxLowpass(newUnit->units[i]); /* specific setup for lowpass filter */
	}

	return newUnit;
}


/* - Highpass-Filter */
unit *addFxHighpass(unitScope scope) {
	unit *newUnit = addUnit(scope); /* adding and setting up basics for new unit*/
	int i, iMax = ((scope == usGLOBAL) ? 1 : voiceCount);

	newUnit->type = utFX_HIGHPASS;
	newUnit->comp = (void (*)(void *))&compFxHighpass; /* setup computing function for unit */

	for (i = 0; i < iMax; i++) { /* setup one inner unit for global unit or one for every voice for local unit */
		newUnit->units[i] = malloc(sizeof(fxHighpass)); /* allocate memory for every inner unit */
		setupFxHighpass(newUnit->units[i]); /* specific setup for highpass filter */
	}

	return newUnit;
}

/* - Bandpass-Filter */
unit *addFxBandpass(unitScope scope) {
	unit *newUnit = addUnit(scope); /* adding and setting up basics for new unit*/
	int i, iMax = ((scope == usGLOBAL) ? 1 : voiceCount);

	newUnit->type = utFX_BANDPASS;
	newUnit->comp = (void (*)(void *))&compFxBandpass; /* setup computing function for unit */

	for (i = 0; i < iMax; i++) { /* setup one inner unit for global unit or one for every voice for local unit */
		newUnit->units[i] = malloc(sizeof(fxBandpass)); /* allocate memory for every inner unit */
		setupFxBandpass(newUnit->units[i]); /* specific setup for bandpass filter */
	}

	return newUnit;
}



/* ---Helpers ---*/

/* - common */
unit *addUnit(unitScope scope) { /* adds a new unit and allocates memory */
	unit *newUnit;
	int i;

	if (scope == usGLOBAL) {
		gUnitCount++;
		gUnits = realloc(gUnits, sizeof(unit *) * gUnitCount); /* gUnit array grows */
		newUnit = gUnits[gUnitCount - 1] = malloc(sizeof(unit));

		newUnit->units = malloc(sizeof(void *)); /* allocate memory for single inner unit */
		newUnit->acts = malloc(sizeof(char *));
		newUnit->acts[0] = gBools + 1; /* sets inner unit to active */
	} else {
		lUnitCount++;
		lUnits = realloc(lUnits, sizeof(unit *) * lUnitCount); /* lUnit array grows */
		newUnit = lUnits[lUnitCount - 1] = malloc(sizeof(unit));

		newUnit->units = malloc(sizeof(void *) * voiceCount); /* allocate memory for every inner unit */
		newUnit->acts = malloc(sizeof(char *) * voiceCount);
		for (i = 0; i < voiceCount; i++) {
			newUnit->acts[i] = gBools + 1; /* set all inner units to active */
		}
	}

	newUnit->scope = scope;

	return newUnit;
}

/* - Params */
float *addGlobalParam() { /* add an allocate new static value */
	gParamCount++;
	gParams = realloc(gParams, sizeof(float *) * gParamCount); /* array grows */
	gParams[gParamCount - 1] = malloc(sizeof(float));
	return gParams[gParamCount - 1];
}

float **getParamAddress(unit *u, paramType type, paramOption option, int i) { /* get address of a value for changing it */
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

char isGlobalParam(float **p) { /* checks if value is global static value */
	int i;

	for (i = 0; i < gParamCount; i++) {
		if (gParams[i] == *p) return 1;
	}

	return 0;
}

float *getValAddress(unit *u, int i) { /* get address of unit output for routing it */

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
char **getBoolParamAddress(unit *u, boolType type, int i) { /* get address of bool value for cahnging it */
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

char *getBoolValAddress(unit *u, int i) { /* get address of unit bool output for routing it */
	
	/* Unit-Type-Bool-Vals*/
	switch (u->type) {
		case utVOICE_ACT: return &(voices[i].act);
		default: return NULL;
	}
}

/* - Oscillators */
float (*getOscFunc(oscType type))(float, float, float) { /* returns pointer to oscillators inner computing function of specified type */

	/* Oscillator Types */
	switch (type) {
		case otSIN: return &oscSin;
		case otTRI: return &oscTri;
		case otREC: return &oscRec;
		default: return NULL;
	}
}
