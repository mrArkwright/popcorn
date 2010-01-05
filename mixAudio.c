#include "mixAudio.h"

#define echoLen 10000

int range = 32;

void mixAudio(void *unused, Uint8 *stream, int len) {
	int i;
	float outputValue;

	for (i=0;i<len;i++) {

		outputValue = fxEcho(compVoices()) * range;

		if (outputValue > 127) outputValue = 127;        /* and clip the result */
		if (outputValue < -128) outputValue = -128;      /* this seems a crude method, but works very well */

		stream[i] = outputValue;
	}
}

float fxEcho(float in) {
	static float preVals[echoLen];
	static float *prePoint = preVals;
	float out = 0.7 * in + 0.3 * *prePoint;

	*prePoint = in;

	prePoint++;
	if (prePoint == preVals + echoLen) prePoint = preVals;
		
	return out;
}
