/*
Filename: main.cpp
Creator:  daoyun
Date: 01-08-2018
Description:
  An example to get the gps data with a class interface .
*/

#include "Kalman.h"
#include "rtk_gps.h"
#include <iostream>
#include <time.h>
#include <unistd.h>

boost::signals2::connection GPS_connection;
boost::signals2::connection IMU_connection;
boost::signals2::connection MAG_connection;
boost::signals2::connection NED_connection;
boost::signals2::connection BaselineNED_connection;

void GPS_Update(rtk_data_t &rtk_data) {
  printf("tow lat lon: %d,%4.10lf,%4.10lf \n", rtk_data.pos_llh.tow,
         rtk_data.pos_llh.lat, rtk_data.pos_llh.lon);
}

int main(int argc, char **argv) {

  char port_name[] = "/dev/ttyUSB0";
  char *port_p = port_name;

  RTK_GPS rtk_gps(port_p);
  rtk_data_t rtk_data;

  GPS_connection = rtk_gps.subscribeToGPS(&GPS_Update);
  //  IMU_connection = rtk_gps.subscribeToIMU(&IMU_Update);
  //  MAG_connection = rtk_gps.subscribeToMAG(&MAG_Update);
  //  NED_connection = rtk_gps.subscribeToNED(&NED_Update);
  //  BaselineNED_connection =
  //  rtk_gps.subscribeToBaselineNED(&BaselineNED_Update);
  //   boost::bind(&IMU_Update,NULL , _1));

  // rtk_gps.init(port_p);
  // rtk_gps.create_thread();
  // RTK_GPS rtk_gps_buf;

  // rtk_data_t rtk_data;
  // rtk_data = rtk_gps.get_gps_data();

  while (1) {
    //  printf("lat: %4.10lf lon:%4.10lf \n ", (*(rtk_gps.pos_llh_p)).lat,
    //  (*(rtk_gps.pos_llh_p)).lon);
    //  printf("vel_ned_n:%d  vel_ned_e:%d \n", (*(rtk_gps.vel_ned_data_p)).n,
    //  (*(rtk_gps.vel_ned_data_p)).e);
    //  printf("heading is %f \n", *(rtk_gps.heading_data_p));
    //    rtk_data = rtk_gps.get_gps_data();
    // printf("tow, gps lat, lon: %d, %lf, %lf \n", rtk_data.pos_llh.tow,
    // rtk_data.pos_llh.lat, rtk_data.pos_llh.lon);
    // printf("tow, imu accx, accy, accz: %d, %d, %d, %d \n",
    // rtk_data.imu_data_raw.tow, rtk_data.imu_data_raw.acc_x,
    // rtk_data.imu_data_raw.acc_y, rtk_data.imu_data_raw.acc_z);
    // printf("mag heading is %f \n", rtk_data.heading_data);
    //  usleep(100000);
  }

  return 0;
}
