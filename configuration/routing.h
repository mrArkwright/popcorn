#ifndef ROUTING_H
#define ROUTING_H

#include "../processing/misc.h"
#include "../processing/oscillators.h"
#include "../processing/compute.h"

float *routeMasterOutput(gUnit *); 

gUnit *addGlobalOsc(int);
gUnit *addGlobalUnit();

void taddOsc();
void taddLOsc();

#endif
