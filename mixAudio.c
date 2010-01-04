#include "mixAudio.h"

int range = 127;

void mixAudio(void *unused, Uint8 *stream, int len) {
	int i;
	float outputValue;

	for (i=0;i<len;i++) {
		
		/*compOscs();*/

		/*outputValue = *globalOutput * range;*/

		compVoices();

		outputValue = *(voices[0].output);

		if (outputValue > 127) outputValue = 127;        /* and clip the result */
		if (outputValue < -128) outputValue = -128;      /* this seems a crude method, but works very well */

		stream[i] = outputValue;
	}
}
