#include "mixAudio.h"

int range = 64;

void mixAudio(void *unused, Uint8 *stream, int len) {
	int i;
	float outputValue;

	for (i=0;i<len;i++) {

		compVoices();

		outputValue = sumVoices() * range;

		if (outputValue > 127) outputValue = 127;        /* and clip the result */
		if (outputValue < -128) outputValue = -128;      /* this seems a crude method, but works very well */

		stream[i] = outputValue;
	}
}

float sumVoices() {
	int i;
	float out = 0;

	for (i = 0; i < voiceCount; i++) {
		if (voices[i].act == 1) {
			out += *(voices[i].output);
		}
	}

	return out;
}
