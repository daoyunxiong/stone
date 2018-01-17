#include "rtk_gps.h"
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv)
{
  char port_name[] = "/dev/ttyUSB0";
  char *port_p = port_name;
 
  RTK_GPS rtk_gps(port_p);
 
 // rtk_gps.init(port_p);
 // rtk_gps.create_thread();
  //RTK_GPS rtk_gps_buf;
  
  rtk_data_t rtk_data;
 // rtk_data = rtk_gps.get_rtk_gps_data();

  while(1)
  {
  //  printf("lat: %4.10lf lon:%4.10lf \n ", (*(rtk_gps.pos_llh_p)).lat, (*(rtk_gps.pos_llh_p)).lon);
 //   printf("vel_ned_n:%d  vel_ned_e:%d \n", (*(rtk_gps.vel_ned_data_p)).n, (*(rtk_gps.vel_ned_data_p)).e);
  //  printf("heading is %f \n", *(rtk_gps.heading_data_p));
    rtk_data = rtk_gps.get_rtk_gps_data();
   // printf("gps lat: %lf  lon: %lf \n", rtk_data.pos_llh.lat, rtk_data.pos_llh.lon);
   // printf("mag heading is %f \n", rtk_data.heading_data);
    sleep(1);
  }

  return 0;
}
