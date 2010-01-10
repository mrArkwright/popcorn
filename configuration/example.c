#include "example.h"

void routeExample() {
	addGlobalOsc(otSIN);

	setGlobalParam(gUnits+0, ptFREQ, poVAL, 440);
	setGlobalParam(gUnits+0, ptFREQ, poRANGE, 100);

	addGlobalOsc(otSIN);
	setGlobalParam(gUnits+1, ptFREQ, poVAL, 1.6);

	routeGlobalParam(gUnits+0, ptFREQ, poMOD, gUnits+1);

	routeMasterOutput(gUnits+0);
}

/*
	gParams[0] = 1;
	gParams[1] = 0;	
	gParams[2] = 10;
	gParams[3] = 0.9;
	gParams[4] = 0.9;
	gParams[5] = 0.5;
	gParams[6] = 1;
	gParams[7] = 0;
	gParams[8] = 0.4;
	gParams[9] = 1;
	gParams[10] = 0;
	gParams[11] = 10;
	gParams[12] = 0.4;
	gParams[13] = -0.7;

void routeVoice(voice* v) {
	initOsc(&(v->oscs[0]), &oscRec, &(v->freq), gParams, gParams+1);
	initParam(&(v->oscs[0].freq), &(v->lfos[0].val), gParams+2);
	initParam(&(v->oscs[0].param1), &(v->lfos[1].val), gParams+3);
	initParam(&(v->oscs[0].vol), &(v->lfos[2].val), gParams+4);

	initOsc(&(v->lfos[0]), &oscTri, gParams+5, gParams+6, gParams+7);

	initOsc(&(v->lfos[1]), &oscSin, gParams+8, gParams+9, gParams+10);

	initOsc(&(v->lfos[2]), &oscSin, gParams+11, gParams+12, gParams+13);
}
*/
