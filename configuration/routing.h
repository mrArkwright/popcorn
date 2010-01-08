#ifndef ROUTING_H
#define ROUTING_H

#include "../processing/voices.h"

#define globParamCount 32

void route();
void initGlobalParams();
void routeVoices();
void routeVoice(voice *);
void routeMaster();

float gParams[globParamCount];
float *masterOutput;

#endif
