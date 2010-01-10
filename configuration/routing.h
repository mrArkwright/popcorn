#ifndef ROUTING_H
#define ROUTING_H

#include "../processing/misc.h"
#include "../processing/oscillators.h"
#include "../processing/compute.h"

float *routeMasterOutput(gUnit *); 

gUnit *addGlobalOsc(int);
void setGlobalParam(gUnit *, int, int, float);
void routeGlobalParam(gUnit *, int, int, gUnit *);

gUnit *addGlobalUnit();
float *addGlobalParam();
float **getGlobalParamAddress(gUnit *, int, int);
float *getGlobalValAddress(gUnit *);
char isParamDefault(float **);

#endif
