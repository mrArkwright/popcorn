/*
 *  main program
 *
 *  for now:
 *  - init audio
 *  - fill buffer
 */

#include <stdio.h>
#include <pthread.h>
#include <SDL.h>

#include <jack/jack.h>

jack_port_t *output_port;
jack_port_t *input_port;

#include "jack.h"
#include "voices.h"

unsigned int sampleRate;

int main(int argc, char *argv[]) {
	jack_client_t *client;

	/* setting up voices */
	initVoices();

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

	while(1) {
		sleep(1);
	}
	jack_client_close(client);

	return 0;
}

