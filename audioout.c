/*
 * Still hacked but with ring buffer
 */
#include <stdio.h>
#include <math.h>
#include <ao/ao.h>

ao_device *aoInit(ao_sample_format *fmt) {
	ao_device *dev
	int def_driver;

	/* init */
	ao_initialize();
	def_driver = ao_default_driver_id();
	/* open device */
	dev = ao_open_live(def_driver, fmt, NULL);
	if(dev = NULL) {
		fprintf(stderr, "Error opening device.\n");
		return NULL;
	}

	return dev;
}

int aoClose(ao_device *dev) {
	ao_close(dev);
	ao_shutdown();
}


