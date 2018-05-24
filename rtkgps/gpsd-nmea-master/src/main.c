/*
 * Author: Sven Kreiss <sk@svenkreiss.com>, Mar 29, 2011
 *
 * This is the test case for the nmeaparser.
 */

#include "gps.h"
#include <stdio.h>

struct gps_data_t my_gps;	// global GPS device

int main(void) {
	// test nmea sentence nmea_parse.c:
	nmea_parse("$GPGGA,123519,4807.038,N,01131.324,E,1,08,0.9,545.4,M,46.9,M,,*42", &my_gps);

	printf("Position: lat %f, lon %f\n", my_gps.fix.latitude, my_gps.fix.longitude);
	return 0;
}
