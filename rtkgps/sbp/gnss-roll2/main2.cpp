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
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

boost::signals2::connection GPS_connection;
boost::signals2::connection IMU_connection; 
boost::signals2::connection MAG_connection;
boost::signals2::connection NED_connection;
boost::signals2::connection BaselineNED_connection;

double OFFSET_X;
double OFFSET_Y;
Mat image_g;
Mat image_head_g;

double vx_g;
double vy_g;
FILE *fp;
//int buf[8][3];
//Madgwick filter;
//MahonyAHRS filter;
//Kalman kalman_filter;

void GPS_Update(rtk_data_t &rtk_data) {

vx_g = rtk_data.vel_ned_data.e;
vy_g = rtk_data.vel_ned_data.n;

static int flag = 1;
static int heading_count;
if(1 == flag)
{
  OFFSET_X = rtk_data.utmcoor.x;
  OFFSET_Y = rtk_data.utmcoor.y;
  flag = 0;
}

//printf("x,y,%f,%f\n", rtk_data.utmcoor.x - OFFSET_X, rtk_data.utmcoor.y - OFFSET_Y);
//printf("tow lat lon: %d,%4.10lf,%4.10lf \n", rtk_data.pos_llh.tow, rtk_data.pos_llh.lat, rtk_data.pos_llh.lon);
fprintf(fp, "%d,%4.10lf,%4.10lf,%d,%d,%f,%f,%d,%d\n",rtk_data.pos_llh.tow, rtk_data.pos_llh.lat, rtk_data.pos_llh.lon, rtk_data.pos_llh.flags, rtk_data.pos_llh.n_sats, rtk_data.heading_data, rtk_data.heading_mag, rtk_data.vel_ned_data.n, rtk_data.vel_ned_data.e);
fflush(fp);

Point gpspoint;
gpspoint.x = rtk_data.utmcoor.x - OFFSET_X + 500;
gpspoint.y = rtk_data.utmcoor.y - OFFSET_Y ;
gpspoint.y = -gpspoint.y +500;
 

if(rtk_data.pos_llh.flags == 4)
  circle(image_g, gpspoint, 1, Scalar(255,0,0));
if(rtk_data.pos_llh.flags == 3)
  circle(image_g, gpspoint, 1, Scalar(0,0,255));
if(rtk_data.pos_llh.flags == 1)
  circle(image_g, gpspoint, 1, Scalar(198,187,155));

heading_count ++;
Point2f heading_gps, heading_mag, heading_kalman;
heading_gps.y = rtk_data.heading_data;
heading_gps.x = heading_count;
heading_mag.y = rtk_data.heading_mag;
heading_mag.x = heading_count;
heading_kalman.x = heading_count;
heading_kalman.y = rtk_data.heading_kalman;


//if(heading_count < 800)
{
  circle(image_head_g, heading_gps, 1, Scalar(0,100,255));
  circle(image_head_g, heading_mag, 1, Scalar(255,100,0));
  circle(image_head_g, heading_kalman, 1, Scalar(100,100,100));
}

//printf("gps,mag,kalman %f %f %f\n", heading_gps.y, heading_mag.y, heading_kalman.y);

if(heading_count > 800)
{
  Mat t_mat = Mat::zeros(2, 3, CV_32FC1);
  t_mat.at<float>(0, 0) = 1; 
  t_mat.at<float>(0, 2) = -1; //水平平移量  
  t_mat.at<float>(1, 1) = 1;
  t_mat.at<float>(1, 2) = 0; //竖直平移量
 // warpAffine(image_head_g, image_head_g, t_mat, image_head_g.size());
  heading_count = 0;

  Mat image(1000,1000,CV_8UC3, Scalar::all(255));
  image_head_g = image.clone();
}

//circle(image_g, gpspoint, 10, Scalar(0,0,255));

imshow("gps", image_g);
imshow("heading", image_head_g);
waitKey(1);
}

void IMU_Update(rtk_data_t &rtk_data) {
    rtk_data_t temp = rtk_data;
    //imu_data_process(rtk_data.imu_data_raw,temp);
    float gps_velocity;
/*    
    cout << rtk_data.vel_ned_data.n << ","
    << rtk_data.vel_ned_data.e << ","
    << rtk_data.imu_data_raw.tow << ","
    << temp.imu_data_raw.acc_x << ","
    << temp.imu_data_raw.acc_y << ","
    << temp.imu_data_raw.acc_z << "," << endl;
*/
    int ax_i = rtk_data.imu_data_raw.acc_x - AX_OFFSET;
    int ay_i = rtk_data.imu_data_raw.acc_y - AY_OFFSET;
    int az_i = rtk_data.imu_data_raw.acc_z - AZ_OFFSET;

  //  vx_g = vx_g + ax_i * 2 / 32768.0 * 0.2;    
  //  vx_g = vy_g + ay_i * 2 / 32768.0 * 0.2;    
   // cout << " vxg vyg" << vx_g << "," << vy_g << endl;
 //   rtk_data.utmcoor.x += vx_g * sin(rtk_data.heading_mag) * 0.2;
 //   rtk_data.utmcoor.y += vy_g * cos(rtk_data.heading_mag) * 0.2;
    //cout << " utmx utmy" << rtk_data.utmcoor.x << "," << rtk_data.utmcoor.y << endl;
//    printf("the value is %d %d %d \n", ax_i, ay_i, az_i);

//  newangle = rtk_data.heading_mag;
//  if((newangle < -90 && angle > 90) || (newangle >90 && angle <-90))
//  {
//    kalman_filter.setAngle(newangle);
//    angle = newangle;
//  }
//  else
//  {
//           angle = kalman_filter.getAngle(newangle, gz, dt);
//  }
//
//#ifndef DEBUG        
//            printf("Orientation: heading=%f  pitch=%f  roll=%f MAG heading=%f kalman=%f ahrs=%f gpsheading=%f\n", heading, pitch, roll, rtk_data.heading_mag, angle, heading_ahrs, rtk_data.heading_data);
//#endif
}

void MAG_Update(rtk_data_t &rtk_data) {
  static int count = 0;
/*
  std::cout  << rtk_data.mag_data_raw.tow << ","
  << rtk_data.mag_data_raw.mag_x << ","
  << rtk_data.mag_data_raw.mag_y << ","
  << rtk_data.mag_data_raw.mag_z << "  ";
*/
//  rtk_data.heading_mag = rtk_data.heading_mag*(180.0/M_PI) + 180;
//  rtk_data.heading_mag = 360.0 - rtk_data.heading_mag;
//  rtk_data.heading_mag -= 90.0;
//  if(rtk_data.heading_mag < 0)
//    rtk_data.heading_mag += 360;
 // printf("mag heading is =%f\n", rtk_data.heading_mag);
}

void NED_Update(rtk_data_t &rtk_data) {
 // printf("vned t n e:%d,%d,%d ", rtk_data.vel_ned_data.tow, rtk_data.vel_ned_data.n, rtk_data.vel_ned_data.e);
}

void BaselineNED_Update(rtk_data_t &rtk_data) {
 // printf("t n e :%d,%d,%d\n", rtk_data.baseline_ned_data.tow, rtk_data.baseline_ned_data.n, rtk_data.baseline_ned_data.e);
}

int main(int argc, char** argv)
{
 // memset(buf, 0 , sizeof(buf));

  Mat image(1000,1000,CV_8UC3, Scalar::all(255));
  image_g = image.clone();
  image_head_g = image.clone();

  fp = fopen("gps.txt", "a+");
   

 char port_name[] = "/dev/ttyUSB0";
 // char port_name[] = "/dev/ttyACM0";
  char *port_p = port_name;
 
  RTK_GPS rtk_gps(port_p);
  rtk_data_t rtk_data;

  GPS_connection = rtk_gps.subscribeToGPS(&GPS_Update);
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
