#ifndef MISC_H
#define MISC_H

#include <stdlib.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define defParamCount 3

typedef enum {utVOICE = 1, utOSC} unitType;
typedef enum {usLOCAL = 1, usGLOBAL} unitScope;
typedef enum {ptFREQ = 1, ptVOL, ptPARAM1} paramType;
typedef enum {poVAL = 1, poMOD, poRANGE} paramOption;
typedef enum {btACT = 1} boolType;
typedef enum {otSIN = 1, otTRI, otREC} oscType;

extern float **gParams;
extern int gParamCount;
extern char gBools[];
extern float defParams[];

extern unsigned int sampleRate;

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
