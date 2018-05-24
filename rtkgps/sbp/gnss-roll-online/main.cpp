/*
Filename: main.cpp
Creator:  daoyun
Date: 01-08-2018
*/
#include "Kalman.h"
#include "rtk_gps.h"
#include <iostream>
#include <math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <unistd.h>

using namespace std;
using namespace cv;

boost::signals2::connection GPS_connection;

double OFFSET_X;
double OFFSET_Y;
Mat image_g;
FILE *fp;

void GPS_Update(rtk_data_t &rtk_data) {
  static int flag = 1;
  if (1 == flag) {
    OFFSET_X = rtk_data.utmcoor.x;
    OFFSET_Y = rtk_data.utmcoor.y;
    flag = 0;
  }

  // printf("tow lat lon: %d,%4.10lf,%4.10lf \n", rtk_data.pos_llh.tow,
  // rtk_data.pos_llh.lat, rtk_data.pos_llh.lon);
  fprintf(fp, "%d,%4.10lf,%4.10lf,%d,%d,%f,%f,%d,%d\n", rtk_data.pos_llh.tow,
          rtk_data.pos_llh.lat, rtk_data.pos_llh.lon, rtk_data.pos_llh.flags,
          rtk_data.pos_llh.n_sats, rtk_data.heading_kalman,
          rtk_data.heading_mag, rtk_data.vel_ned_data.n,
          rtk_data.vel_ned_data.e);
  fflush(fp);
}

int main(int argc, char **argv) {
  Mat image(1000, 1000, CV_8UC3, Scalar::all(255));
  image_g = image.clone();
  fp = fopen("gps.txt", "a+");

  char port_name[] = "/dev/ttyUSB0";
  char *port_p = port_name;
  RTK_GPS rtk_gps(port_p);
  rtk_data_t rtk_data;

  GPS_connection = rtk_gps.subscribeToGPS(&GPS_Update);

  while (1) {
    usleep(10000);
  }

  fclose(fp);
  return 0;
}
