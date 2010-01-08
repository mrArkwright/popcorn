#include "routing.h"

void route() {
	initGlobalParams();
	routeVoices();
	routeMaster();
}

void initGlobalParams() {
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
	gParams[14] = 3000;

}

void routeVoices() {
	int i;

	globalVoiceSettings.oscCount = 2;
	globalVoiceSettings.lfoCount = 3;

	initVoices();
	
	for (i = 0; i < voiceCount; i++) {
		routeVoice(voices + i);
	}
}

void routeMaster() {
	masterOutput = &voicesOutput;

	initOsc(&gLfo, &oscSin, gParams+9, gParams+14, gParams+1);
}

void routeVoice(voice* v) {
	initOsc(&(v->oscs[0]), &oscRec, &(v->freq), gParams, gParams+1);
	/*initParam(&(v->oscs[0].freq), &(v->lfos[0].val), gParams+2);*/
	initParam(&(v->oscs[0].param1), &(v->lfos[1].val), gParams+3);
	initParam(&(v->oscs[0].vol), &(v->lfos[2].val), gParams+4);

	initOsc(&(v->lfos[0]), &oscTri, gParams+5, gParams+6, gParams+7);

	initOsc(&(v->lfos[1]), &oscSin, gParams+8, gParams+9, gParams+10);

	initOsc(&(v->lfos[2]), &oscSin, gParams+11, gParams+12, gParams+13);

	v->output = &(v->oscs[0].val);
}
