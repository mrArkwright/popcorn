/*
 * some simple oscillators with complex routing
 *
 */
#include <stdio.h>
#include <math.h>
#include <SDL.h>

extern unsigned int sampleRate;
/*
unsigned int audioBufferSize = 0;
unsigned int outputAudioBufferSize = 0;
*/

typedef struct {
	float val;
	float *mod;
	float range;
} param;

typedef struct {
	char act;
	float val;
	float phase;
	float (*func)(float, float, float);
	param freq, vol, param1;
} osc;

void oscRoute();
void initOsc(osc*, float(*)(float, float, float), float, float, float);
void initParam(param*, float*, float);

void example_mixaudio(void*, Uint8*, int);

void compOsc(osc*);
float compParam(param*);

float oscSin(float, float, float);
float oscTri(float, float, float);
float oscRec(float, float, float);

