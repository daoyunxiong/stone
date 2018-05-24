//  Example C++ gpsd program using libgpsmm


#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <csignal>
#include <sstream>
#include <unistd.h>
#include <libgpsmm.h>
using namespace std;

void signalHandler( int signum ) {
   cout << "Interrupt signal (" << signum << ") received.\n";

   // cleanup and close up stuff here  
   // terminate program  

   exit(signum);  
}

int main(void)
{
//register signal SIGINT and signal handler  
  signal(SIGINT, signalHandler); 

  gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

  if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == NULL) {
    std::cerr << "No GPSD running.\n";
    return 1;
  }

  for (;;) {
    struct gps_data_t *gpsd_data;
    
    if (!gps_rec.waiting(100000)) continue;
 //   printf("ok\n");
    if ((gpsd_data = gps_rec.read()) == NULL) {
      std::cerr << "GPSD read error.\n";
      return 1;
    } else {
      while (((gpsd_data = gps_rec.read()) == NULL) ||
             (gpsd_data->fix.mode < MODE_2D)) {
        // Do nothing until fix
      }
      
      timestamp_t ts   = gpsd_data->fix.time;
      double latitude  = gpsd_data->fix.latitude;
      double longitude = gpsd_data->fix.longitude;
      double track     = gpsd_data->fix.track;

      std::cout << ts << " ";
      // convert GPSD's timestamp_t into time_t
      time_t seconds;
      seconds = (time_t)ts;
      auto tm = *std::localtime(&seconds);

      std::ostringstream oss;
      oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
      auto time_str = oss.str();

//      printf("status %d\n", gpsd_data->status);
      // set decimal precision
      std::cout.precision(10);
    //  std::cout.setf(std::ios::fixed, std::ios::floatfield);
      std::cout << time_str << "," <<
        latitude << "," <<
        longitude << "," << std::endl;
   //     "status:" << gpsd_data->status << "," <<
  //      "track:" << gpsd_data->fix.track << "," <<
   //     "speed:" << gpsd_data->fix.speed << "," <<
    //    "num:" << gpsd_data->satellites_used << "," <<
     //   "mode:" <<gpsd_data->fix.mode << "," <<
     //   "epy:" << gpsd_data->fix.epy << "," <<
      //  "epx:" << gpsd_data->fix.epx << "," <<
      //  gpsd_data->rtcm3.length << "," <<
     //   "end" << '\n';
  gps_rec.clear_fix();
    }
  }

  return 0;
}
