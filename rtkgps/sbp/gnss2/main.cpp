#include "rtk_gps.h"
#include <time.h>
#include <unistd.h>

int main(int argc, char** argv)
{
  char port_name[] = "/dev/ttyUSB0";
  char *port_p = port_name;
 
  RTK_GPS rtk_gps(port_p);

  while(1)
  {
    printf("lat: %4.10lf lon:%4.10lf \n ", (*(rtk_gps.pos_llh_p)).lat, (*(rtk_gps.pos_llh_p)).lon);
    sleep(1);
  }

  return 0;
}
