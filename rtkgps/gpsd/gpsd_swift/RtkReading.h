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
#include <cstring>
#include <functional>

#define GPS_POS_PORT 2900
#define GPS_HEADING_PORT 2901

using namespace std;
typedef struct {
  double local_time;
  UTMCoor utm;
  WGS84Corr llh_t;
  double dual_antenna_heading;
  int status;
  int satelites_num;
  double gdop;
} rtk_data_t;

class RtkReading {
public:
  RtkReading(char *port_pos, char *port_heading);
  ~RtkReading();

 rtk_data_t get_rtk_data();

private:
  int set_rtk_data(int flag);
  void process(int flag);
  int init_gpsd(char *port_pos, char *port_heading);
  int close_gpsd();
  rtk_data_t rtk_data;
  std::thread *Gps_thread_pos;
  std::thread *Gps_thread_heading;
  bool threadActive;
  pthread_rwlock_t rwlock;
};

#endif
