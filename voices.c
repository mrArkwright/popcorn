#include "voices.h"

float ia = 1, ib = 0, ic = 10;
float fa = 0.9, fb = 0.5, fc = 0.4, fd = -0.7;

void initVoices() {
	int i;

	for (i = 0; i < voiceCount; i++) {
		initVoice(voices + i);
	}
}

void initVoice(voice* v) {
	v->act = 0;

	routeVoice(v);
}

void routeVoice(voice* v) {
	initOsc(&(v->oscs[0]), &oscRec, &(v->freq), &ia, &ib);
	initParam(&(v->oscs[0].freq), &(v->lfos[0].val), &ic);
	initParam(&(v->oscs[0].param1), &(v->lfos[1].val), &fa);
	initParam(&(v->oscs[0].vol), &(v->lfos[2].val), &fa);

	initOsc(&(v->lfos[0]), &oscTri, &fb, &ia, &ib);

	initOsc(&(v->lfos[1]), &oscSin, &fc, &ia, &ib);

	initOsc(&(v->lfos[2]), &oscTri, &ic, &ia, &fd);

	v->output = &(v->oscs[0].val);
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
	int i;

	for (i = 0; i < voiceCount; i++) {
		if (voices[i].act == 0) {
			voices[i].freq = getFreq(note);
			voices[i].velocity = velocity;
			voices[i].act = 1;
			break;
		}
	}
}

void stopSound(int note, int velocity) {
	int i;
	float freq = getFreq(note);

	for (i = 0; i < voiceCount; i++) {
		if ((voices[i].act == 1) && (voices[i].freq == freq)) {
			voices[i].act = 0;
		}
	}
}

float getFreq(int note) {
	return 440 * pow(2, (note - 69) / 12.0);
}
