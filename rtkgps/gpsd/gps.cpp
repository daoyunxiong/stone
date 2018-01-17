//  Example C++ gpsd program using libgpsmm


#include <iostream>
#include <fstream>
#include <iomanip>
#include <ctime>
#include <sstream>

#include <libgpsmm.h>

int main(void)
{
  gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

  if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == NULL) {
    std::cerr << "No GPSD running.\n";
    return 1;
  }

  for (;;) {
    struct gps_data_t *gpsd_data;

    if (!gps_rec.waiting(100000)) continue;

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

      // convert GPSD's timestamp_t into time_t
      time_t seconds;
      seconds = (time_t)ts;
      auto tm = *std::localtime(&seconds);

      std::ostringstream oss;
      oss << std::put_time(&tm, "%d-%m-%Y %H:%M:%S");
      auto time_str = oss.str();

      // set decimal precision
      std::cout.precision(10);
      std::cout.setf(std::ios::fixed, std::ios::floatfield);
      std::cout << time_str << "," <<
        latitude << "," <<
        longitude << "," <<
        track << '\n';

    }
  }

  return 0;
}
