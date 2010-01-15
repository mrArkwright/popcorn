#include <stdio.h>
#include <stdlib.h>
#include <jack/jack.h>
#include <jack/midiport.h>

#include "processing/compute.h"

int jackProcess(jack_nframes_t nframes, void *arg) {
	extern jack_port_t *output_port, *input_port;
	unsigned int i;
	/* buffers */
	void* midiBuf;
	jack_default_audio_sample_t *outBuf;
	/* midi event */
	jack_midi_event_t midiEvent;
	jack_nframes_t midiEventIndex = 0;
	jack_nframes_t midiEventCount;

	midiBuf = jack_port_get_buffer(input_port, nframes);
	outBuf = (jack_default_audio_sample_t *) jack_port_get_buffer(output_port, nframes);

	midiEventCount = jack_midi_get_event_count(midiBuf);

	/* print events to stdout	
	if(midiEventCount > 1) {
		printf(" popcorn: have %d events \n", midiEventCount);
		for(i=0; i<midiEventCount; i++) {
			jack_midi_event_get(&midiEvent, midiBuf, i);
			printf("\tevent %d time is %d. 1st byte is 0x%x\n", i, midiEvent.time, *(midiEvent.buffer));
		}
	}
	*/
	jack_midi_event_get(&midiEvent, midiBuf, 0);
	for(i=0; i<nframes; i++ ) {
		if( (midiEvent.time == i) && (midiEventIndex < midiEventCount)) {
			
			/* play / stop note */
			if( (*(midiEvent.buffer) & 0xf0) == 0x90) {
				startVoice( *(midiEvent.buffer + 1), 128);
			}
			else if( (*(midiEvent.buffer) & 0xf0) == 0x80) {
				stopVoice( *(midiEvent.buffer + 1), 128);
			}

			midiEventIndex++;
			if(midiEventIndex < midiEventCount)
				jack_midi_event_get(&midiEvent, midiBuf, midiEventIndex);

		}
		compute();
		outBuf[i] =  *masterOutput;
	}
	return 0;
}

int jackSRateChanged(jack_nframes_t nframes, void *arg) {
	extern unsigned int sampleRate;
	unsigned int new = 0;

	/* reset global sample rate */
	if((new = (jack_default_audio_sample_t) nframes) != 0)
		sampleRate = new;

	fprintf(stderr, "the sample rate is now %" PRIu32 "/sec\n", nframes);

	return 0;
}

void jackShutdown(void *arg) {
	exit(1);
}
