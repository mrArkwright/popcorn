#include "mixAudio.h"

int range = 32;

void mixAudio(void *unused, Uint8 *stream, int len) {
	int i;
	float outputValue;

	for (i=0;i<len;i++) {

		compVoices();

		outputValue = *masterOutput * range;

		if (outputValue > 127) outputValue = 127;        /* and clip the result */
		if (outputValue < -128) outputValue = -128;      /* this seems a crude method, but works very well */

		stream[i] = outputValue;
	}
}
