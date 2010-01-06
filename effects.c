#include "effects.h"

#define echoLen 10000
#define firLen 27

#define iirWidth 200

float firVals[27] = {-0.001880, -0.002858, -0.004162, -0.005049, -0.004196, 0.0, 0.008932, 0.023254, 0.042477, 0.064831, 0.087477, 0.107033, 0.120302, 0.125, 0.120302, 0.107033, 0.087477, 0.064831, 0.042477, 0.023254, 0.008932, 0.0, -0.004196, -0.005049, -0.004162, -0.002858, -0.001880};

/*float firVals[27] = {0.000051, -0.000800, -0.003291, 0.003531, 0.017011, 0.0, -0.041199, -0.022013, 0.061445, 0.063549, -0.057838, -0.106554, 0.023903, 0.125, 0.023903, -0.106554, -0.057838, 0.063549, 0.061445, -0.022013, -0.041199, 0.0, 0.017011, 0.003531, -0.003291, -0.000800, 0.000051};*/

/*float firVals[5] = {0.1, 0.42, 0.003, 0.23, 0.032};*/

float fxEcho(float in) {
	static float preVals[echoLen];
	static float *prePoint = preVals;
	float out = 0.7 * in + 0.3 * *prePoint;

	*prePoint = in;

	prePoint++;
	if (prePoint == preVals + echoLen) prePoint = preVals;
		
	return out;
}

float fxFIR(float in) {
	static float preVals[firLen];
	static float *prePoint = preVals;

	float *prePoint2 = prePoint;
	float val, out = 0;
	int i;

	for (i = 0; i < firLen; i++) {
		val = *prePoint2;
		out += firVals[i] * val;

		prePoint2--;
		if (prePoint2 == preVals - 1) prePoint2 += firLen;
	}

	*prePoint = in;

	prePoint++;
	if (prePoint == preVals + firLen) prePoint = preVals;
		
	return out;
}

float fxIIR(float in) {
	static float hpOut, lpOut;

	float hpCoeffOmega = 2.0 * M_PI * (iirCutoff + iirWidth) / sampleRate;
	float hpCoeff = 2.0 - cos(hpCoeffOmega) - sqrt(pow(2.0 - cos(hpCoeffOmega), 2.0) - 1.0);

	float lpCoeffOmega = 2.0 * M_PI * (iirCutoff - iirWidth) / sampleRate;
	float lpCoeff = 2.0 - cos(lpCoeffOmega) - sqrt(pow(2.0 - cos(lpCoeffOmega), 2.0) - 1.0);
	
	hpOut = (1 - hpCoeff) * in + hpCoeff * hpOut;
	lpOut = (1 - lpCoeff) * (in - hpOut) + lpCoeff * lpOut;

	return lpOut;
}
