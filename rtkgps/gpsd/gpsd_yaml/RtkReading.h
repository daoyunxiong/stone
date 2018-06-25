/*************************************************************************
        > File Name: RtkReading.h
        > Author: ma6174
        > Mail: ma6174@163.com
        > Created Time: Wed 13 Jun 2018 02:33:24 PM CST
 ************************************************************************/
#ifndef RTKREADING_H
#define RTKREADING_H

#include "utm.h"
#include <libgpsmm.h>
#include <unistd.h>
#include <thread>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <functional>

#include "yaml-cpp/emitter.h"
#include "yaml-cpp/node/emit.h"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/impl.h"
#include "yaml-cpp/node/convert.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/detail/impl.h"
#include "yaml-cpp/node/parse.h"
#define GPS_POS_PORT 2900
#define GPS_HEADING_PORT 2901

using namespace std;
typedef struct {
  double local_time;
  double gps_time;
  double latitude;
  double longitude;
  double altitude;
  double utmx;
  double utmy;
  int pos_status;
  int heading_status;
  double reference_heading; 
  double attitude_heading;
  int gps_mode;
  UTMCoor utm;
  WGS84Corr llh_t;
  int status;
  int satelites_num;
  double gdop;
} rtk_data_t;

class RtkReading {
public:
  RtkReading(char* file_path);
  ~RtkReading();

 rtk_data_t get_rtk_data();

private:
  double getCurrentTicks();
  int set_rtk_data(int flag);
  int set_rtk_heading_data(int flag);
  void process(int flag);
  void process_heading(int flag);
  int init_gpsd(char *port_pos, char *port_heading);
  int close_gpsd();
  rtk_data_t rtk_data;
  std::thread *Gps_thread_pos;
  std::thread *Gps_thread_heading;
  bool threadActive;
  pthread_rwlock_t rwlock;
  int port_num;
  int antenna_num;
  int gpsd_wait_time;
};

#endif
