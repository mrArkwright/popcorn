#ifndef MISC_H
#define MISC_H

#include <stdlib.h>
#include <math.h>

#define defParamCount 3

enum unitType {utOSC = 1};
enum unitScope {usLOCAL = 1, usGLOBAL};
enum paramType {ptFREQ = 1, ptVOL, ptPARAM1};
enum paramOption {poVAL = 1, poMOD, poRANGE};
enum boolType {btACT = 1};
enum oscType {otSIN = 1, otTRI, otREC};

extern float **gParams;
extern int gParamCount;
extern char gBools[];
extern float defParams[];

extern unsigned int sampleRate;
extern float m2pi;

typedef struct {
	float *val;
	float *mod;
	float *range;
} param;


/* --- Setup --- */

void setupParam(param *);


/* --- Computing --- */

float compParam(param*);


/* Stuff */

float getFreq(int);

#endif
