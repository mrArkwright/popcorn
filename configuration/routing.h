#ifndef ROUTING_H
#define ROUTING_H

#include "../processing/misc.h"
#include "../processing/oscillators.h"
#include "../processing/compute.h"

/* --- API --- */

void routeMasterOutput(unit *);
void routeVoicesOutput(unit *);

void setParam(unit *, int, int, float);
void routeParam(unit *, int, int, unit *);


/* ---Helper ---*/

float *addGlobalParam();
float **getParamAddress(unit *, int, int, int);
char **getBoolAddress(unit *, int, int);
char isGlobalParam(float **);
float *getValAddress(unit *, int);

#endif
