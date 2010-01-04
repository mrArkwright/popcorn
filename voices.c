#include "voices.h"

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
	initOsc(&(v->oscs[0]), &oscRec, 440, 1, 0);
	initParam(&(v->oscs[0].freq), &(v->lfos[0].val), 10);
	initParam(&(v->oscs[0].param1), &(v->lfos[1].val), 0.9);
	initParam(&(v->oscs[0].vol), &(v->lfos[2].val), 0.9);

	initOsc(&(v->lfos[0]), &oscTri, 0.5, 1, 0);

	initOsc(&(v->lfos[1]), &oscSin, 0.4, 1, 0);

	initOsc(&(v->lfos[2]), &oscTri, 10, 1, -0.7);

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
	voices[0].note = note;
	voices[0].velocity = velocity;
	voices[0].act = 1;
}

void stopSound(int note, int velocity) {
	voices[0].act = 0;
}
