#include "rtk_gps.h"
#include "MadgwickAHRS.h"
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

#define AX_OFFSET 437
#define AY_OFFSET 383
#define AZ_OFFSET -17954
#define GX_OFFSET 102
#define GY_OFFSET 29
#define GZ_OFFSET 112

float convertRawAcceleration(int aRaw)
{
    // since we are using 2G range
    // -2g maps to a raw value of -32768
    // +2g maps to a raw value of 32767

    float a = (aRaw * 2.0) / 32768.0;
    return a;
}

float convertRawGyro(int gRaw)
{
    // since we are using 250 degrees/seconds range
    // -250 maps to a raw value of -32768
    // +250 maps to a raw value of 32767

    float g = (gRaw * 500.0) / 32768.0;
    return g;
}

extern struct rtk_data_t rtk_data;

int main(int argc, char** argv)
{
  char port_name[] = "/dev/ttyUSB0";
//  char port_name[] = "/dev/ttyACM0";
  char *port_p = port_name;
 
  RTK_GPS rtk_gps(port_p);
  Madgwick filter;
  
  struct timeval tv;
 
 // rtk_gps.init(port_p);
 // rtk_gps.create_thread();
  //RTK_GPS rtk_gps_buf;
  
  //rtk_data_t rtk_data;
 // rtk_data = rtk_gps.get_rtk_gps_data();

  int ax_i, ay_i, az_i;
  int gx_i, gy_i, gz_i;
  int mx_i, my_i, mz_i;
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float roll, pitch, heading;

  int real_time;
  while(1)
  {
 //   printf("vel_ned_n:%d  vel_ned_e:%d \n", (*(rtk_gps.vel_ned_data_p)).n, (*(rtk_gps.vel_ned_data_p)).e);
  //  printf("heading is %f \n", *(rtk_gps.heading_data_p));
   // rtk_data = rtk_gps.get_rtk_gps_data();
    usleep(20001);
   // printf("tow, lat, lon: %d, %4.10lf,%4.10lf \n ", rtk_data.pos_llh.tow, rtk_data.pos_llh.lat, rtk_data.pos_llh.lon);
   // printf("tow,n_sats,flags %d,%d,%d \n", (int)rtk_data.pos_llh.tow, (int)rtk_data.pos_llh.n_sats, (int)rtk_data.pos_llh.flags);    
  // printf("acc %d %f %f %f \n",  rtk_data.imu_data.tow, rtk_data.imu_data_cal.acc_xx, rtk_data.imu_data_cal.acc_yy, rtk_data.imu_data_cal.acc_zz);
  // printf("acc %d %d %d %d \n",  rtk_data.imu_data.tow, rtk_data.imu_data.acc_x, rtk_data.imu_data.acc_y, rtk_data.imu_data.acc_z);
   //usleep(20000);
   continue;
    //real_time = clock();
    gettimeofday(&tv, NULL);
    ax_i = rtk_data.imu_data.acc_x - AX_OFFSET;
    ay_i = rtk_data.imu_data.acc_y - AY_OFFSET;
    az_i = rtk_data.imu_data.acc_z - AZ_OFFSET + 16384;
    gx_i = rtk_data.imu_data.gyr_x - GX_OFFSET;
    gy_i = rtk_data.imu_data.gyr_y - GY_OFFSET;
    gz_i = rtk_data.imu_data.gyr_z - GZ_OFFSET;
    mx_i = rtk_data.mag_data.mag_x;
    my_i = rtk_data.mag_data.mag_y;
    mz_i = rtk_data.mag_data.mag_z;
    
//    printf("%ld.%ld, %d, %d,%d,%d,%d,%d,%d,%d,%d,%d \n",tv.tv_sec, tv.tv_usec / 1000, rtk_data.imu_data.tow, ax_i, ay_i, az_i, gx_i, gy_i, gz_i, mx_i, my_i, mz_i);
//    continue;

    ax = convertRawAcceleration(ax_i);
    ay = convertRawAcceleration(ay_i);
    az = convertRawAcceleration(az_i);
    gx = convertRawGyro(gx_i);
    gy = convertRawGyro(gy_i);
    gz = convertRawGyro(gz_i);

    filter.updateIMU(gx, gy, gz, ax, ay, az);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();

   // printf("Orientation: heading=%f  pitch=%f  roll=%f\n", heading, pitch, roll);

    //printf("%f,%f,%f,%f,%f,%f,%f,%f,%f \n", ax, ay, az, gx, gy, gz, mx, my, mz);
    printf("%ld.%ld, %d, %d,%d,%d,%d,%d,%d,%d,%d,%d \n",tv.tv_sec, tv.tv_usec / 1000, rtk_data.imu_data.tow, ax_i, ay_i, az_i, gx_i, gy_i, gz_i, mx_i, my_i, mz_i);
      
   // printf("gps lat: %lf  lon: %lf \n", rtk_data.pos_llh.lat, rtk_data.pos_llh.lon);
   // printf("mag heading is %f \n", rtk_data.heading_data);
    usleep(20000);
  }

  return 0;
}
