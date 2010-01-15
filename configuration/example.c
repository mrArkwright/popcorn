#include "example.h"

void routeExample() {
	initRouting();

	/* adding and setting up units */
	addOsc(usLOCAL);
	setOscType(lUnits[0], otREC);
	setParam(lUnits[0], ptPARAM1, poRANGE, 0.9);
	setParam(lUnits[0], ptVOL, poRANGE, 0.9);

	addOsc(usLOCAL);
	setParam(lUnits[1], ptFREQ, poVAL, 0.4);

	addOsc(usLOCAL);
	setParam(lUnits[2], ptFREQ, poVAL, 10);
	setParam(lUnits[2], ptVOL, poVAL, 0.4);
	setParam(lUnits[2], ptPARAM1, poVAL, -0.7);

	/* route params */
	routeParam(lUnits[0], ptPARAM1, poMOD, lUnits[1]);
	routeParam(lUnits[0], ptVOL, poMOD, lUnits[2]);

	/* route params to voices */
	routeParam(lUnits[0], ptFREQ, poVAL, &voiceFreq);
	routeBool(lUnits[0], btACT, &voiceActive);

	/* route output */
	routeVoicesOutput(lUnits[0]);
	masterOutput = &voicesOutput;
	/*routeMasterOutput(gUnits[0]);*/
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
	//initParam(&(v->oscs[0].freq), &(v->lfos[0].val), gParams+2);
	initParam(&(v->oscs[0].param1), &(v->lfos[1].val), gParams+3);
	initParam(&(v->oscs[0].vol), &(v->lfos[2].val), gParams+4);

	//initOsc(&(v->lfos[0]), &oscTri, gParams+5, gParams+6, gParams+7);

	initOsc(&(v->lfos[1]), &oscSin, gParams+8, gParams+9, gParams+10);

	initOsc(&(v->lfos[2]), &oscSin, gParams+11, gParams+12, gParams+13);
}
*/
