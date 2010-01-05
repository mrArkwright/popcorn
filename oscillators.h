/*
 * some simple oscillators with complex routing
 *
 */
#include <stdio.h>
#include <math.h>

extern unsigned int sampleRate;

typedef struct {
	float *val;
	float *mod;
	float *range;
} param;

typedef struct {
	char act;
	float val;
	float phase;
	float (*func)(float, float, float);
	param freq, vol, param1;
	char restartOnPlay;
} osc;

void initOsc(osc*, float(*)(float, float, float), float*, float*, float*, char);
void initParam(param*, float*, float*);

void compOsc(osc*);
float compParam(param*);

float oscSin(float, float, float);
float oscTri(float, float, float);
float oscRec(float, float, float);
