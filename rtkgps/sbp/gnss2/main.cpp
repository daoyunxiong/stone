#include "rtk_gps.h"
#include "MadgwickAHRS.h"
#include <time.h>
#include <unistd.h>

#define AX_OFFSET 10
#define AY_OFFSET 10
#define AZ_OFFSET 10
#define GX_OFFSET 10
#define GY_OFFSET 10
#define GZ_OFFSET 10

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

    float g = (gRaw * 250.0) / 32768.0;
    return g;
}


int main(int argc, char** argv)
{
 // char port_name[] = "/dev/ttyUSB0";
  char port_name[] = "/dev/ttyACM0";
  char *port_p = port_name;
 
  RTK_GPS rtk_gps(port_p);
  Madgwick filter;
 
 // rtk_gps.init(port_p);
 // rtk_gps.create_thread();
  //RTK_GPS rtk_gps_buf;
  
  rtk_data_t rtk_data;
 // rtk_data = rtk_gps.get_rtk_gps_data();

  int ax_i, ay_i, az_i;
  int gx_i, gy_i, gz_i;
  int mx_i, my_i, mz_i;
  float ax, ay, az;
  float gx, gy, gz;
  float mx, my, mz;
  float roll, pitch, heading;


  while(1)
  {
  //  printf("lat: %4.10lf lon:%4.10lf \n ", (*(rtk_gps.pos_llh_p)).lat, (*(rtk_gps.pos_llh_p)).lon);
 //   printf("vel_ned_n:%d  vel_ned_e:%d \n", (*(rtk_gps.vel_ned_data_p)).n, (*(rtk_gps.vel_ned_data_p)).e);
  //  printf("heading is %f \n", *(rtk_gps.heading_data_p));
    rtk_data = rtk_gps.get_rtk_gps_data();

    ax_i = rtk_data.imu_data.acc_x;
    ay_i = rtk_data.imu_data.acc_y;
    az_i = rtk_data.imu_data.acc_z;
    gx_i = rtk_data.imu_data.gyr_x;
    gy_i = rtk_data.imu_data.gyr_y;
    gz_i = rtk_data.imu_data.gyr_z;
    mx_i = rtk_data.mag_data.mag_x;
    my_i = rtk_data.mag_data.mag_y;
    mz_i = rtk_data.mag_data.mag_z;
    
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

    printf("%f,%f,%f,%f,%f,%f,%f,%f,%f \n", ax, ay, az, gx, gy, gz, mx, my, mz);
    printf("%d,%d,%d,%d,%d,%d,%d,%d,%d \n", ax_i, ay_i, az_i, gx_i, gy_i, gz_i, mx_i, my_i, mz_i);
      
   // printf("gps lat: %lf  lon: %lf \n", rtk_data.pos_llh.lat, rtk_data.pos_llh.lon);
   // printf("mag heading is %f \n", rtk_data.heading_data);
    usleep(100000);
  }

  return 0;
}
