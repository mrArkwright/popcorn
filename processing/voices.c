#include "voices.h"

int voiceCount;
voice *voices = NULL;
voice *firstVoice, *lastVoice;
float actVoices;


/* --- Setup --- */

void setupVoices() {
	int i;

	actVoices = 0;

	voices = realloc(voices, sizeof(voice) * voiceCount);

	for (i = 0; i < voiceCount; i++) {
		setupVoice(voices + i);
	}
}

void setupVoice(voice* v) {
	v->act = 0;
	v->preVoice = NULL;
	v->postVoice = NULL;

	v->output = defParams + 0;
}


/* Control */

void startVoice(int note, int velocity) {
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
	newVoice->velocity = velocity / 128.0;
	newVoice->act = 1;
}

void stopVoice(int note, int velocity) {
	int i;
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
			voices[i].velocity = velocity / 128.0;
			actVoices--;
		}
	}
}
