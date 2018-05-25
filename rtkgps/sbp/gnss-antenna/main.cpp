 // waitKey(1);
/*
Filename: main.cpp
Creator:  daoyun
Date: 01-08-2018
Description:
  An example to get the gps data with a class interface .
*/

#include "rtk_gps.h"
#include <time.h>
#include <unistd.h>
#include <iostream>
#include "Kalman.h"

#include <math.h>

using namespace std;

boost::signals2::connection GPS_connection;

FILE *fp;

void GPS_Update(rtk_data_t &rtk_data) {
  fprintf(fp, "%d,%4.10lf,%4.10lf,%lf,%lf,%d,%d,%f,%f,%d,%d,%lf\n",rtk_data.pos_llh.tow, rtk_data.pos_llh.lat, rtk_data.pos_llh.lon, rtk_data.utmcoor.x, rtk_data.utmcoor.y, rtk_data.pos_llh.flags, rtk_data.pos_llh.n_sats, rtk_data.heading_data, rtk_data.heading_mag, rtk_data.vel_ned_data.n, rtk_data.vel_ned_data.e, (float)(rtk_data.baseline_heading_data.heading) / 1000);
  fflush(fp);
  printf("%d,%4.10lf,%4.10lf,%d,%d,%f,%f,%d,%d,%f\n",rtk_data.pos_llh.tow, rtk_data.pos_llh.lat, rtk_data.pos_llh.lon, rtk_data.pos_llh.flags, rtk_data.pos_llh.n_sats, rtk_data.heading_data, rtk_data.heading_mag, rtk_data.vel_ned_data.n, rtk_data.vel_ned_data.e, (float)(rtk_data.baseline_heading_data.heading) / 1000);
}


int main(int argc, char** argv)
{

  fp = fopen("gps.txt", "a+");

  char port_angle[] = "/dev/ttyUSB0";
  char port_gps[] = "/dev/ttyACM0";
  char *port_p = port_gps;
 
  RTK_GPS rtk_gps(port_gps, 1);
  RTK_GPS rtk_gps_angle(port_angle, 0);
//  rtk_data_t rtk_data;

  //GPS_connection = rtk_gps.subscribeToGPS(&GPS_Update);

  while(1)
  {
	  usleep(1000000);
  }

  return 0;
}
