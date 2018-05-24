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

boost::signals2::connection GPS_connection;
boost::signals2::connection IMU_connection; 
boost::signals2::connection MAG_connection;
boost::signals2::connection NED_connection;
boost::signals2::connection BaselineNED_connection;

int ax_i, ay_i, az_i;
int gx_i, gy_i, gz_i;
int mx_i, my_i, mz_i;
float ax, ay, az;
float gx, gy, gz;
float mx, my, mz;
float roll = 0;
float pitch = 0;
float heading = 0;

float newangle;
float dt = 0.02;
float angle;


int buf[8][3];
//Madgwick filter;
//MahonyAHRS filter;
Kalman kalman_filter;

void GPS_Update(rtk_data_t &rtk_data) {
printf("tow lat lon: %d,%4.10lf,%4.10lf \n", rtk_data.pos_llh.tow, rtk_data.pos_llh.lat, rtk_data.pos_llh.lon);

}

void IMU_Update(rtk_data_t &rtk_data) {
    rtk_data_t temp = rtk_data;
    //imu_data_process(rtk_data.imu_data_raw,temp);
 /*   std::cout  << rtk_data.imu_data_raw.tow << ","
             << temp.imu_data_raw.acc_x << ","
             << temp.imu_data_raw.acc_y << ","
             << temp.imu_data_raw.acc_z << ","
             << temp.imu_data_raw.gyr_x << ","
             << temp.imu_data_raw.gyr_y << ","
             << temp.imu_data_raw.gyr_z << "\n";
*/
            ax_i = rtk_data.imu_data_raw.acc_x - AX_OFFSET;
            ay_i = rtk_data.imu_data_raw.acc_y - AY_OFFSET;
            az_i = rtk_data.imu_data_raw.acc_z - AZ_OFFSET - 16384 ;//16384
            gx_i = rtk_data.imu_data_raw.gyr_x - GX_OFFSET;
            gy_i = rtk_data.imu_data_raw.gyr_y - GY_OFFSET;
            gz_i = rtk_data.imu_data_raw.gyr_z - GZ_OFFSET;
            mx_i = rtk_data.mag_data_raw.mag_x - MAGNETOMETER_ORIGIN_X;
            my_i = rtk_data.mag_data_raw.mag_y - MAGNETOMETER_ORIGIN_Y;
            mz_i = rtk_data.mag_data_raw.mag_z - MAGNETOMETER_ORIGIN_Z;
        //    if(abs(gx_i) < 10) gx_i = 0;
        //    if(abs(gy_i) < 10) gy_i = 0;
        //    if(abs(gz_i) < 10) gz_i = 0;
#ifdef DEBUG
        printf("tow acc gyo mag %d, %d,%d,%d,%d,%d,%d,%d,%d,%d \n", rtk_data.imu_data_raw.tow, ax_i, ay_i, az_i, gx_i, gy_i, gz_i, mx_i, my_i, mz_i);
#endif
            ax = RTK_GPS::convertRawAcceleration(ax_i);
            ay = RTK_GPS::convertRawAcceleration(ay_i);
            az = RTK_GPS::convertRawAcceleration(az_i);
            gx = RTK_GPS::convertRawGyro(gx_i);
            gy = RTK_GPS::convertRawGyro(gy_i);
            gz = RTK_GPS::convertRawGyro(gz_i);
            mx = - (float) mx_i;
            my = - (float) my_i;
            mz = - (float) mz_i;

            heading = heading + gz * 0.02;
           filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);
           //filter.updateIMU(gx, gy, gz, ax, ay, az);

            // print the heading, pitch and roll
            roll = filter.getRoll();
            pitch = filter.getPitch();
           float  heading_ahrs = filter.getYaw();

           newangle = rtk_data.heading_mag;
  if((newangle < -90 && angle > 90) || (newangle >90 && angle <-90))
  {
    kalman_filter.setAngle(newangle);
    angle = newangle;
  }
  else
  {
           angle = kalman_filter.getAngle(newangle, gz, dt);
  }

#ifndef DEBUG        
            printf("Orientation: heading=%f  pitch=%f  roll=%f MAG heading=%f kalman=%f ahrs=%f gpsheading=%f\n", heading, pitch, roll, rtk_data.heading_mag, angle, heading_ahrs, rtk_data.heading_data);
#endif

}

void MAG_Update(rtk_data_t &rtk_data) {
  static int count = 0;
 
/*
  std::cout  << rtk_data.mag_data_raw.tow << ","
  << rtk_data.mag_data_raw.mag_x << ","
  << rtk_data.mag_data_raw.mag_y << ","
  << rtk_data.mag_data_raw.mag_z << "  ";
*/
  mx_i = rtk_data.mag_data_raw.mag_x - MAGNETOMETER_ORIGIN_X;
  my_i = rtk_data.mag_data_raw.mag_y - MAGNETOMETER_ORIGIN_Y;
  mz_i = rtk_data.mag_data_raw.mag_z - MAGNETOMETER_ORIGIN_Z;

  buf[count][0] = mx_i;
  buf[count][1] = my_i;
  buf[count][2] = mz_i;

  float sumx = 0;
  float sumy = 0;
  for(int j = 0; j<8; j++)
  {
    sumx += buf[j][0];
    sumy += buf[j][1];
  }

  mx_i = sumx / 8;
  my_i = sumy / 8;
  
  rtk_data.heading_mag = atan2(-my_i, mx_i) * 180.0 / M_PI;
//  rtk_data.heading_mag = rtk_data.heading_mag*(180.0/M_PI) + 180;
//  rtk_data.heading_mag = 360.0 - rtk_data.heading_mag;
//  rtk_data.heading_mag -= 90.0;
//  if(rtk_data.heading_mag < 0)
//    rtk_data.heading_mag += 360;
 // printf("mag heading is =%f\n", rtk_data.heading_mag);

  count ++;
  if(count > 7)
    count = 0;
}

void NED_Update(rtk_data_t &rtk_data) {
  printf("vned t n e:%d,%d,%d ", rtk_data.vel_ned_data.tow, rtk_data.vel_ned_data.n, rtk_data.vel_ned_data.e);
}

void BaselineNED_Update(rtk_data_t &rtk_data) {
  printf("t n e :%d,%d,%d\n", rtk_data.baseline_ned_data.tow, rtk_data.baseline_ned_data.n, rtk_data.baseline_ned_data.e);
}

int main(int argc, char** argv)
{
  memset(buf, 0 , sizeof(buf));

  char port_name[] = "/dev/ttyUSB0";
  char *port_p = port_name;
 
  RTK_GPS rtk_gps(port_p);
  rtk_data_t rtk_data;

//GPS_connection = rtk_gps.subscribeToGPS(&GPS_Update);
  IMU_connection = rtk_gps.subscribeToIMU(&IMU_Update);
  MAG_connection = rtk_gps.subscribeToMAG(&MAG_Update);
//  NED_connection = rtk_gps.subscribeToNED(&NED_Update);
//  BaselineNED_connection = rtk_gps.subscribeToBaselineNED(&BaselineNED_Update);
         //   boost::bind(&IMU_Update,NULL , _1));
 
 // rtk_gps.init(port_p);
 // rtk_gps.create_thread();
  //RTK_GPS rtk_gps_buf;
  
  //rtk_data_t rtk_data;
  //rtk_data = rtk_gps.get_gps_data();

  while(1)
  {
  //  printf("lat: %4.10lf lon:%4.10lf \n ", (*(rtk_gps.pos_llh_p)).lat, (*(rtk_gps.pos_llh_p)).lon);
  //  printf("vel_ned_n:%d  vel_ned_e:%d \n", (*(rtk_gps.vel_ned_data_p)).n, (*(rtk_gps.vel_ned_data_p)).e);
  //  printf("heading is %f \n", *(rtk_gps.heading_data_p));
//    rtk_data = rtk_gps.get_gps_data();
   // printf("tow, gps lat, lon: %d, %lf, %lf \n", rtk_data.pos_llh.tow, rtk_data.pos_llh.lat, rtk_data.pos_llh.lon);
   // printf("tow, imu accx, accy, accz: %d, %d, %d, %d \n", rtk_data.imu_data_raw.tow, rtk_data.imu_data_raw.acc_x, rtk_data.imu_data_raw.acc_y, rtk_data.imu_data_raw.acc_z);
   // printf("mag heading is %f \n", rtk_data.heading_data);
  //  usleep(100000);
  }

  return 0;
}
