#include "voices.h"

voice voices[voiceCount];
voice *firstVoice, *lastVoice;
float actVoices;

float voicesOutput;


/* --- Initialization --- */

void initVoices() {
	int i;

	actVoices = 0;

	for (i = 0; i < voiceCount; i++) {
		initVoice(voices + i);
	}
}

void initVoice(voice* v) {
	int i;

	v->act = 0;
	v->preVoice = NULL;
	v->postVoice = NULL;

	v->oscs = (osc *) malloc(sizeof(osc) * globalVoiceSettings.oscCount);
	v->lfos = (osc *) malloc(sizeof(osc) * globalVoiceSettings.lfoCount);

	for (i = 0; i < globalVoiceSettings.oscCount; i++) {
		v->oscs[i].init = 0;
	}

	for (i = 0; i < globalVoiceSettings.lfoCount; i++) {
		v->lfos[i].init = 0;
	}
}


/* --- Computing --- */

void compVoices() {
	int i;

	voicesOutput = 0;

	for (i = 0; i < voiceCount; i++) {
		if (voices[i].act == 1) { /* voice auch berechnen wenn voice aus (wegen nachhall etc)??? */
			compVoice(&(voices[i]));
			voicesOutput += *(voices[i].output);
		}
	}
}

void compVoice(voice* v) {
	int i;

	for (i = 0; i < globalVoiceSettings.oscCount; i++) {
		if (v->oscs[i].act == 1) {
			compOsc(&(v->oscs[i]));
		}
	}

	for (i = 0; i < globalVoiceSettings.lfoCount; i++) {
		if (v->lfos[i].act == 1) {
			compOsc(&(v->lfos[i]));
		}
	}
}


/* Control */

void playSound(int note, int velocity) {
	int i;
	voice* newVoice;

	if (actVoices < voiceCount) {
		for (i = 0; i < voiceCount; i++) {
			if (voices[i].act == 0) {
				newVoice = &(voices[i]);
				break;
			}
		}
		if (actVoices == 0) {
			firstVoice = newVoice; /* ist immer das erste arrayelement ... if fisrtvoice==null...kann beim stop gesetzt werden? */
			lastVoice = newVoice;
		}
		actVoices++;
	} else {
		newVoice = firstVoice;
		firstVoice = firstVoice->postVoice;
	}
	
	newVoice->preVoice = lastVoice;
	lastVoice->postVoice = newVoice;
	lastVoice = newVoice;

	newVoice->freq = getFreq(note);
	newVoice->velocity = velocity;
	newVoice->act = 1;

	for (i = 0; i < globalVoiceSettings.oscCount; i++) {
		startOsc(newVoice->oscs + i);
	}

	for (i = 0; i < globalVoiceSettings.lfoCount; i++) {
		startOsc(newVoice->lfos + i);
	}
}

void stopSound(int note, int velocity) {
	int i, j;
	float freq = getFreq(note);

	for (i = 0; i < voiceCount; i++) {
		if (voices[i].freq == freq && voices[i].act == 1) {

			if ((voices + i) == firstVoice) {
				firstVoice = firstVoice->postVoice;
			} else {
				voices[i].preVoice->postVoice = voices[i].postVoice;
			}
			if ((voices + i) == lastVoice) {
				lastVoice = lastVoice->preVoice;
			} else {
				voices[i].postVoice->preVoice = voices[i].preVoice;
			}

			voices[i].act = 0;
			actVoices--;

			for (j = 0; j < globalVoiceSettings.oscCount; j++) {
				stopOsc(voices[i].oscs + j);
			}

			for (j = 0; j < globalVoiceSettings.lfoCount; j++) {
				stopOsc(voices[i].lfos + j);
			}
		}
	}
}
