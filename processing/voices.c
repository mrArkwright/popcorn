#include "voices.h"

int voiceCount; /* number of voices to setup and compute */
voice *voices = NULL; /* array which holds all voices */
voice *firstVoice, *lastVoice; /* pointers to the first and last voice that is playing */
float actVoices; /* number of currently playing voices */


/* --- Setup --- */

void setupVoices() { /* setup specified number of voices */
	int i;

	actVoices = 0; /* no voices are playing */

	voices = realloc(voices, sizeof(voice) * voiceCount); /* allocate memory for voices TODO: write allocate wrappers (new global misc file!!) */

	for (i = 0; i < voiceCount; i++) {
		setupVoice(voices + i);
	}
}

void setupVoice(voice* v) { /* setup single voice */
	v->act = 0;
	v->preVoice = NULL;
	v->postVoice = NULL;

	v->output = defParams + 0;
}


/* Control */

void startVoice(int note, int velocity) { /* start playing a voice */
	int i;
	voice* newVoice; /* voice to use */

	if (actVoices < voiceCount) { /* not all voices are playing -> use the next free */
		for (i = 0; i < voiceCount; i++) {
			if (voices[i].act == 0) {
				newVoice = &(voices[i]);
				break;
			}
		}

		if (actVoices == 0) { /* no other voices are playing -> current is the first and last started voice */
			firstVoice = newVoice;
			lastVoice = newVoice;
		}

		actVoices++;
	} else { /* all voices are playing -> use first started voice */
		newVoice = firstVoice;
		firstVoice = firstVoice->postVoice; /* the voice started after the first is now the first started */
	}
	
	/* add current voice to the end */
	newVoice->preVoice = lastVoice;
	lastVoice->postVoice = newVoice;
	lastVoice = newVoice;

	/* set values for current voice */
	newVoice->freq = getFreq(note);
	newVoice->velocity = velocity / 128.0;
	newVoice->act = 1;

#ifdef DEBUG_VOICES
	debugVoices();
#endif
}

void stopVoice(int note, int velocity) { /* stop all voices with specific note */
	int i;
	float freq = getFreq(note);

	for (i = 0; i < voiceCount; i++) {
		if (voices[i].freq == freq && voices[i].act == 1) { /* find all playing voices with specific note */

			if ((voices + i) == firstVoice) { /* found voice is first stated -> new one is now first */
				firstVoice = firstVoice->postVoice;
			} else { /* found voice isn't first -> set next voice of preceding voice to next voice of found voice */
				voices[i].preVoice->postVoice = voices[i].postVoice;
			}

			if ((voices + i) == lastVoice) { /* found voice is last voice -> preceding voice is now last voice */
				lastVoice = lastVoice->preVoice;
			} else { /* found voice isn't last voice -> set preceding voice of next voice to preceding voice of found voice */
				voices[i].postVoice->preVoice = voices[i].preVoice;
			}

			/* stop found voices and set release-velocity */
			voices[i].act = 0;
			voices[i].velocity = velocity / 128.0;
			actVoices--;
		}
	}

#ifdef DEBUG_VOICES
	debugVoices();
#endif
}

#ifdef DEBUG_VOICES
void debugVoices() {
	int i, pre, post;

	for (i = 0; i < voiceCount; i++) {
		printf("voice%2d: ", i);
		printf("act: %d   ", voices[i].act);

		pre = (voices[i].preVoice == NULL) ? -1 : (voices[i].preVoice - voices);
		printf("pre: voice%2d   ", pre);

		post = (voices[i].postVoice == NULL) ? -1 : (voices[i].postVoice - voices);
		printf("post: voice%2d   ", post);

		printf("freq: %5f   ", voices[i].freq);
		printf("vel: %f   ", voices[i].velocity);
		printf("\n");
	}

	printf("\n");
}
#endif
