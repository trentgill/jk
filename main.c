// standard C libs
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h> /* for atoi */

#include <jack/jack.h> // Jack headers
// #include <lo/lo.h> // Liblo headers

// File headers
#include "main.h"

// private variables
jack_client_t *client;

jack_default_audio_sample_t *delay_line;
jack_nframes_t delay_index;
jack_nframes_t latency = 1024;

jack_port_t *input_ports[IN_PORTS];
jack_port_t *output_ports[OUT_PORTS];


// private fns

int32_t process_block (jack_nframes_t nframes, void *arg)
{
	// audio i/o buffers
	jack_default_audio_sample_t* jack_in[IN_PORTS];
	jack_default_audio_sample_t* jack_out[OUT_PORTS];
	int32_t ch;
	
	// get input buffers from jack (nb: reverse 'output' symbol)
	for (ch=0; ch < IN_PORTS; ch++) {
		jack_out[ch] = jack_port_get_buffer (output_ports[ch], nframes);
	}

	// get an output buffers (reserved memory) to send output back to jack
	for (ch=0; ch < OUT_PORTS; ch++) {
		jack_in[ch] = jack_port_get_buffer (input_ports[ch], nframes);
	}

	// NB: THESE FNS ARE SUPER WEIRD, THEY SEND LEFT THEN RIGHT CHANS
	// IN SEQUENCE, RATHER THAN AS A PAIR. HOW TO DO THIS IN A GENERIC WAY?
	// PERHAPS JUST SEND A POINTER TO THE BEGINNING OF THE FRAME BLOCKS.
	// THIS IS WHAT A CODEC DOES, SO YOU NEED THE SAME SERIALIZATION GLUE.

	// NEED TO SETTLE ON A GENERIC FORM SO IT CAN BE ROLLED INTO
	// PRODUCTION CODE WITHOUT REQUIRING MODIFICATION BETWEEN SIM & SHIP


#ifdef SINGLE_SAMPLE /* single-sample processing */
	for (int32_t i=0; i < nframes; i++) {
		for (ch=0; ch < IN_PORTS; ch++) {
			// module_process_frame is extern
			// define it in your dsp code!
			*(jack_out[ch] + i) = module_process_frame( *(jack_in[ch] + i) );
		}
	}
#else /* native block-processing */
	for (ch=0; ch < IN_PORTS; ch++) {
		// module_process_frame is extern
		// define it in your dsp code
		module_process_frame( jack_in[ch], jack_out[ch], nframes );
	}
#endif /* end of block process */

	return 0;
}

void jack_shutdown (void *arg)
{
	fprintf(stderr, "JACK shut down, exiting ...\n");
	exit (1);
}

// private declarations
/*int32_t foo_handler(const uint8_t *path, const uint8_t *types, lo_arg ** argv,
						int32_t argc, void *data, void *user_data);
int32_t generic_handler(const uint8_t *path, const uint8_t *types, lo_arg ** argv,
						int32_t argc, void *data, void *user_data);*/
void error(int32_t num, const uint8_t *m, const uint8_t *path);

// exported fns
int32_t main (int32_t argc, uint8_t *argv[])
{
	const char **ports;
	const char *client_name = "jkkk";
	const char *server_name = NULL;

	
	jack_options_t options = JackNullOption;
	jack_status_t status;

	// use jack latency to create a sample buffer
	if (argc == 2) { latency = atoi(argv[1]); }
	delay_line = malloc( latency * sizeof(jack_default_audio_sample_t));
	if (delay_line == NULL) { fprintf (stderr, "no memory"); exit(1); }
	memset( delay_line, 0, latency * sizeof(jack_default_audio_sample_t));

	// open a client connection to the JACK server
	client = jack_client_open (client_name, options, &status, server_name);
	printf("START!\n");
	if (client == NULL) {
		fprintf (stderr, "jack_client_open() failed, status = 0x%2.0x\n", status);
	    if (status & JackServerFailed) {
			fprintf (stderr, "Unable to connect to JACK server\n");
		}
		exit (1);
	}
	if (status & JackServerStarted) { fprintf (stderr, "JACK server started\n"); }
	if (status & JackNameNotUnique) {
		client_name = jack_get_client_name(client);
		fprintf (stderr, "unique name `%s' assigned\n", client_name);
	}

	// Tell Jack server to run process_block() when there's work to be done
	jack_set_process_callback (client, process_block, 0);

    // tell Jack server to call jack_shutdown() if it needs to exit
	jack_on_shutdown (client, jack_shutdown, 0);

	printf ("engine sample rate: %" PRIu32 "\n", jack_get_sample_rate (client));

  /* create the ports */
	// create Jack ports
	int32_t j;
	for (j=0; j < IN_PORTS; j++) {
		uint8_t foo[100]; // str_buf
		sprintf(foo, "input%d", j);
		input_ports[j] = jack_port_register (client, foo,
							JACK_DEFAULT_AUDIO_TYPE,
							JackPortIsInput, 0);
		if (input_ports[j] == NULL) {
			fprintf(stderr, "no more JACK ports available\n");
			exit (1);
		}
	}
	for (j=0; j < IN_PORTS; j++) {
		uint8_t foo[100];
		sprintf(foo, "output%d", j);
		output_ports[j] = jack_port_register (client, foo,
							JACK_DEFAULT_AUDIO_TYPE,
							JackPortIsOutput, 0);
		if (output_ports[j] == NULL) {
			fprintf(stderr, "no more JACK ports available\n");
			exit (1);
		}
	}

	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
		exit (1);
	}

/*#ifdef WIN32
	Sleep (-1);
#else
	sleep (-1);
#endif
*/
	/* this is never reached but if the program
		had some other way to exit besides being killed,
		they would be important to call.
	*/

	jack_client_close (client);
	exit (0);
}

/*int32_t generic_handler(const uint8_t *path, const uint8_t *types, lo_arg ** argv,
						int32_t argc, void *data, void *user_data)
{
	printf("path: <%s>\n", path);
	for (int32_t i = 0; i < argc; i++) {
		printf("arg %d '%c' ", i, types[i]);
		lo_arg_pp((lo_type)types[i], argv[i]);
		printf("\n");
	}
	printf("\n");
	fflush(stdout);

	return 1;
}

int32_t foo_handler(const uint8_t *path, const uint8_t *types, lo_arg ** argv,
					int32_t argc, void *data, void *user_data) {
	printf("received param %d %d\n", argv[0]->i, argv[1]->i);
	module_set_param(argv[0]->i, argv[1]->i);
	return 0;
}*/


void error(int num, const uint8_t *msg, const uint8_t *path)
{
	printf("liblo server error %d in path %s: %s\n", num, path, msg);
	fflush(stdout);
}
