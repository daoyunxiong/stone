#include "rtk_gps.h"
#include <thread>

using namespace std;
/*
class RTK_GPS
{
public:
  RTK_GPS(char* port);
  ~RTK_GPS();

public:
  msg_gps_time_t     *gps_time_p;
  msg_pos_llh_t      *pos_llh_p;
  msg_baseline_heading_dep_a_t   *pos_heading_p;
  msg_imu_raw_t   *imu_data_p;
  msg_mag_raw_t   *mag_data_p;

public:
  int  create_thread();

};
*/
RTK_GPS::RTK_GPS()
{
  gps_time_p = &gps_time;
  pos_llh_p = &pos_llh;
  pos_heading_p = &pos_heading;
  imu_data_p = &imu_data; 
  mag_data_p = &mag_data; 
  vel_ned_data_p = &vel_ned_data; 
 
  heading_data_p = &heading_data;
  heading_mag_p = &heading_mag;
 
 // open_serial_port(port);
 // setup_port();
 // setup_sbp();

 // create_thread();
 // init(port);
}

RTK_GPS::~RTK_GPS()
{

}

int RTK_GPS::init(char* port)
{
  open_serial_port(port);
  setup_port();
  setup_sbp();

  this->create_thread();
  return 0;
}

int RTK_GPS::create_thread()
{
  std::thread t(process);
  t.detach();
  return 0;
}

