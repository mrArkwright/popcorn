/*
 *  
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <jack/jack.h>

#include "configuration/json.h"
#include "processing/voices.h"
#include "jack.h"

jack_port_t *output_port;
jack_port_t *input_port;

unsigned int sampleRate;

typedef enum {KALL=1, HELP, QUIT, UNKWN} MOptions;
MOptions getopt(void);

void usage(void) {
	printf("(1) Kill all voices\n");
	printf("(2) This Help\n");
	printf("(3) Quit\n");
}

int main(int argc, char *argv[]) {
	MOptions mopt;
	jack_client_t *client;

	/* route */
	routing();

	/* jack */
	client = jack_client_new("popcorn");
	if(client == NULL) {
		fprintf(stderr, "jack server not running?\n");
		return -1;
	}

	jack_set_process_callback(client, jackProcess, 0);
	jack_set_sample_rate_callback(client, jackSRateChanged, 0);
	jack_on_shutdown(client, jackShutdown, 0);

	output_port = jack_port_register(client, "audio_out", JACK_DEFAULT_AUDIO_TYPE, JackPortIsOutput, 0);
	input_port = jack_port_register(client, "midi_in", JACK_DEFAULT_MIDI_TYPE, JackPortIsInput, 0);


	if(jack_activate(client)) {
		fprintf(stderr, "cannot activate client");
		return 1;
	}
	/* END jack */

	usage();
	while((mopt = getopt()) != EOF) {
		switch(mopt) {
			case KALL:
				killVoices();
				break;
			case HELP:
				usage();
				break;
			case QUIT:
				jack_client_close(client);
				exit(0);
				break;

		}
	}

	jack_client_close(client);

	return 0;
}
