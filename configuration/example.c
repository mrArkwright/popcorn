#include "example.h"

void rExample1() {
	/* adding and setting up units */
	addOsc(usLOCAL);
	setOscType(lUnits[0], otREC);
	setParam(lUnits[0], ptPARAM1, poRANGE, 0.9);
	setParam(lUnits[0], ptVOL, poRANGE, 0.9);

	addOsc(usLOCAL);
	setParam(lUnits[1], ptFREQ, poVAL, 0.4);

	addOsc(usLOCAL);
	setParam(lUnits[2], ptFREQ, poVAL, 10);
	setParam(lUnits[2], ptVOL, poVAL, 0.4);
	setParam(lUnits[2], ptPARAM1, poVAL, -0.7);

	/* route params */
	routeParam(lUnits[0], ptPARAM1, poMOD, lUnits[1]);
	routeParam(lUnits[0], ptVOL, poMOD, lUnits[2]);

	/* route params to voices */
	routeParam(lUnits[0], ptFREQ, poVAL, voiceFreq);
	routeParam(lUnits[0], ptVOL, poVAL, voiceVelocity);
	routeBool(lUnits[0], btACT, voiceActive);

	/* route output */
	routeVoicesOutput(lUnits[0]);
	routeMasterOutput(voicesOut);
}

void rExample2() {
	addOsc(usLOCAL);
	setOscType(lUnits[0], otREC);

	routeParam(lUnits[0], ptFREQ, poVAL, voiceFreq);
	routeBool(lUnits[0], btACT, voiceActive);

	routeVoicesOutput(lUnits[0]);
	routeMasterOutput(voicesOut);
}

void rExample3() {
	/* adding and setting up units */
	addOsc(usLOCAL);
	setOscType(lUnits[0], otREC);
	setParam(lUnits[0], ptPARAM1, poRANGE, 0.9);
	setParam(lUnits[0], ptVOL, poRANGE, 0.9);

	addOsc(usLOCAL);
	setParam(lUnits[1], ptFREQ, poVAL, 0.4);

	addOsc(usLOCAL);
	setParam(lUnits[2], ptFREQ, poVAL, 10);
	setParam(lUnits[2], ptVOL, poVAL, 0.4);
	setParam(lUnits[2], ptPARAM1, poVAL, -0.7);

	addOsc(usLOCAL);
	setOscType(lUnits[3], otTRI);
	setParam(lUnits[3], ptPARAM1, poVAL, 0.3);
	setParam(lUnits[3], ptVOL, poVAL, 4);
	setParam(lUnits[3], ptFREQ, poMOD, 5);

	addMixer2ch(usLOCAL);
	setBool(lUnits[4], btACT1, 1);
	setBool(lUnits[4], btACT2, 1);

	/* route params */
	routeParam(lUnits[0], ptPARAM1, poMOD, lUnits[1]);
	routeParam(lUnits[0], ptVOL, poMOD, lUnits[2]);

	routeParam(lUnits[4], ptINPUT1, poVAL, lUnits[0]);
	routeParam(lUnits[4], ptINPUT2, poVAL, lUnits[3]);

	/* route params to voices */
	routeParam(lUnits[0], ptFREQ, poVAL, voiceFreq);
	routeParam(lUnits[0], ptVOL, poVAL, voiceVelocity);
	routeBool(lUnits[0], btACT, voiceActive);

	routeParam(lUnits[3], ptFREQ, poVAL, voiceFreq);
	routeBool(lUnits[3], btACT, voiceActive);

	/* route output */
	routeVoicesOutput(lUnits[4]);
	routeMasterOutput(voicesOut);
}


void routeExample() {
	setupRouting();

	setVoiceCount(4);

	rExample1();
}
