#include "effects.h"

#define echoLen 10000

float fxEcho(float in) {
	static float preVals[echoLen];
	static float *prePoint = preVals;
	float out = 0.7 * in + 0.3 * *prePoint;

	*prePoint = in;

	prePoint++;
	if (prePoint == preVals + echoLen) prePoint = preVals;
		
	return out;
}
