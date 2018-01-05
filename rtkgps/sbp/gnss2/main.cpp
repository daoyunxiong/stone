#include "rtk_gps.h"
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv)
{
  char port_name[] = "/dev/ttyUSB0";
  char *port_p = port_name;
 
  RTK_GPS rtk_gps;
  rtk_gps.init(port_p);
 // rtk_gps.create_thread();
  RTK_GPS rtk_gps_buf;

  while(1)
  {
  //  printf("lat: %4.10lf lon:%4.10lf \n ", (*(rtk_gps.pos_llh_p)).lat, (*(rtk_gps.pos_llh_p)).lon);
 //   printf("vel_ned_n:%d  vel_ned_e:%d \n", (*(rtk_gps.vel_ned_data_p)).n, (*(rtk_gps.vel_ned_data_p)).e);
  //  printf("heading is %f \n", *(rtk_gps.heading_data_p));
    printf("mag heading is %f \n", *(rtk_gps_buf.heading_mag_p));
    sleep(1);
  }

  return 0;
}
