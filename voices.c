#include "voices.h"

void initVoices() {
	int i;

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

	for (i = 0; i < voiceCount; i++) {
		initVoice(voices + i, i);
	}
}

void initVoice(voice* v, int note) {
	v->act = 0;
	v->freq = getFreq(note);

	routeVoice(v);
}

void routeVoice(voice* v) {
	initOsc(&(v->oscs[0]), &oscRec, &(v->freq), gParams, gParams+1, 0);
	/*initParam(&(v->oscs[0].freq), &(v->lfos[0].val), gParams+2);*/
	initParam(&(v->oscs[0].param1), &(v->lfos[1].val), gParams+3);
	initParam(&(v->oscs[0].vol), &(v->lfos[2].val), gParams+4);

	initOsc(&(v->lfos[0]), &oscTri, gParams+5, gParams+6, gParams+7, 1);

	initOsc(&(v->lfos[1]), &oscSin, gParams+8, gParams+9, gParams+10, 0);

	initOsc(&(v->lfos[2]), &oscSin, gParams+11, gParams+12, gParams+13, 0);

	v->output = &(v->oscs[0].val);
}

float sumVoices() {
	int i;
	float out = 0;

	compVoices();

	for (i = 0; i < voiceCount; i++) {
		if (voices[i].act == 1) {
			out += *(voices[i].output);
		}
	}

	return out;
}


void compVoices() {
	int i;

	for (i = 0; i < voiceCount; i++) {
		if (voices[i].act == 1) {
			compVoice(&(voices[i]));
		}
	}
}

void compVoice(voice* v) {
	int i;

	for (i = 0; i < oscCount; i++) {
		if (v->oscs[i].act == 1) {
			compOsc(&(v->oscs[i])); /* compute all active oscillators */
		}
	}

	for (i = 0; i < lfoCount; i++) {
		if (v->lfos[i].act == 1) {
			compOsc(&(v->lfos[i])); /* compute all active lfo's */
		}
	}
}

void playSound(int note, int velocity) {
	if (note >= 0 && note < voiceCount) {
		voices[note].velocity = velocity;
		voices[note].act = 1;
	}
}

void stopSound(int note, int velocity) {
	int i;

	voices[note].act = 0;
			
	for (i = 0; i < lfoCount; i++) {
		if (voices[note].lfos[i].restartOnPlay == 1) {
			voices[note].lfos[i].phase = 0;
		}
	}
}

float getFreq(int note) {
	return 440 * pow(2, (note - 69) / 12.0);
}
