//  Example C++ gpsd program using libgpsmm
#include <cmath>
#include <csignal>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <libgpsmm.h>
#include <memory.h>
#include <sstream>
#include <thread>
#include <unistd.h>

#define ONE_GPS 1
using namespace std;

bool gReadGps = true;
void signalHandler(int signum) {
  cout << "Interrupt signal (" << signum << ") received." << endl;
  gReadGps = false;
}

void read_gps(gpsmm *gps_rec) {
  cout << gps_rec << endl;

  while (gReadGps) {
    struct gps_data_t *gpsd_data;
    if (!gps_rec->waiting(100000))
      continue;
    if ((gpsd_data = gps_rec->read()) == NULL) {
      std::cout << "GPSD read error.\n";
      return;
    } else {
      while (((gpsd_data = gps_rec->read()) == NULL) ||
             (gpsd_data->fix.mode < MODE_2D)) {
        // Do nothing until fix
        ;
      }

      std::cout.precision(12);
      cout << "status:" << gpsd_data->status << ","
           << "track:" << gpsd_data->fix.track << ","
           << "speed:" << gpsd_data->fix.speed << ","
           << "time:" << gpsd_data->fix.time << ","
           << "mode:" << gpsd_data->fix.mode << ","
           << "num:" << gpsd_data->satellites_used << ","
           << "heading:" << gpsd_data->attitude.heading << "," << endl;
    }
    gps_rec->clear_fix();
  }
}

int main(void) {
  // register signal SIGINT and signal handler
  signal(SIGINT, signalHandler);

  char port_pos[] = "/dev/ttyACM0";
  //  char port_heading[] = "/dev/ttyUSB0";
  char cmd[100];
  memset(cmd, 0, sizeof(cmd));
  sprintf(cmd, "gpsd %s  -F /var/run/gpsd_pos.sock -S 2900", port_pos);
  system(cmd);
  //  memset(cmd, 0, sizeof(cmd));
  //  sprintf(cmd, "gpsd %s -n -F /var/run/gpsd_heading.sock -S 2901",
  //          port_heading);
  //  system(cmd);

  usleep(100000);
  gpsmm gps_rec_pos("localhost", "2900");
  // gpsmm gps_rec_heading("localhost", "2901");

  //  if (gps_rec_pos.stream(WATCH_ENABLE | WATCH_JSON) == NULL ||
  //      gps_rec_heading.stream(WATCH_ENABLE | WATCH_JSON) == NULL) {
  if (gps_rec_pos.stream(WATCH_ENABLE | WATCH_JSON) == NULL) {
    std::cerr << "No GPSD running.\n";
    return 1;
  }

  std::thread thread_pos(read_gps, &gps_rec_pos);
  //  std::thread thread_heading(read_gps, &gps_rec_heading);
  if (thread_pos.joinable())
    thread_pos.join();
  //  if(thread_heading.joinable())
  //    thread_heading.join();

  //  while (gReadGps) {
  //    usleep(100000);
  //  }
  system("killall gpsd");
  cout << "end of test" << endl;
  /*
     for (;;) {
     struct gps_data_t *gpsd_data;

     if (!gps_rec.waiting(100000))
     continue;
  //   printf("ok\n");
  if ((gpsd_data = gps_rec.read()) == NULL) {
  std::cerr << "GPSD read error.\n";
  return 1;
  } else {
  while (((gpsd_data = gps_rec.read()) == NULL) ||
  (gpsd_data->fix.mode < MODE_2D)) {
  // Do nothing until fix
  }

  timestamp_t ts = gpsd_data->fix.time;
  double latitude = gpsd_data->fix.latitude;
  double longitude = gpsd_data->fix.longitude;
  double track = gpsd_data->fix.track;

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
  std::cout << time_str << "," << latitude << "," << longitude << ","
  << std::endl;
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
  */
  return 0;
}
