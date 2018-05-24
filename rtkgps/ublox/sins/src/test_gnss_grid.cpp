#include <glog/logging.h>
#include <gflags/gflags.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <sys/time.h> // for gettimeofday
#include <iomanip>
#include "my_kbhit.h"
//#include "debug.h"
#include "gnss_grid.h"

#include <opencv2/highgui/highgui.hpp>
#include <sstream>
#include <string>
#include <mutex>
#include <queue>
#include <signal.h>
#include <perceptin_gen1_interface.h>

using namespace PIRoBot;

#define xm_origin_x 2494563.7
#define xm_origin_y 38493931.7
#define szu_origin_x 2493429.768
#define szu_origin_y 38493408.335

DEFINE_string(serialName, "/dev/ttyACM0", "serial port name");

bool isExit = false;
void intSigintHandler(int sig) {
  isExit = true;
}

// ./ublox_reader --log_dir=. --v=1
int main(int argc, char *argv[])
{
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, false);
  google::InstallFailureSignalHandler();

//  PI::PerceptInGen1Interface::ImuData imu_data;
//  PI::PerceptInGen1Interface interface(PI::PerceptInGen1Interface::StereoFrameRate::STEREO_FRAME_RATE_30HZ, true);

//  if (!interface.StartStreaming()) {
//    std::cout<<("Failed to stream PerceptIn Gen1 device.\n")<<std::endl;
//    return -1;
//  }
/*
  Gnss_grid mGnss_grid;
  bool ret = mGnss_grid.Init(FLAGS_serialName);
  if(!ret)
    return -1;

  mGnss_grid.set_origin(xm_origin_x,xm_origin_y,1);
  signal(SIGINT, intSigintHandler);
  float signal_quality,hdop;
*/  
/*
  while(!isExit)
  {
    mGnss_grid.RunOnce();

    mGnss_grid.get_signal_quality(&signal_quality,&hdop);

    std::cout<<signal_quality<<','<<hdop<<std::endl;

    if(interface.GetImuData(&imu_data)) 
    {
      //std::cout<<("get imu.\n")<<std::endl;
      std::cout<<imu_data.timestamp<<','<<imu_data.accel(0)<<','<<imu_data.accel(1)<<','<<imu_data.accel(2)<<','<<imu_data.gyro(0)<<','<<imu_data.gyro(1)<<','<<imu_data.gyro(2)<<std::endl;
    }
    else
    {
      std::cout<<("no imu.\n")<<std::endl;
    }
    usleep(10000);
  }
*/

// {
 Gnss_grid mGnss_grid;
 int ret = mGnss_grid.Init(FLAGS_serialName);
  if(!ret)
    return -1;

  mGnss_grid.start();
 // init_kbhit();

  while(1) {
    usleep(1000);
//    if(kbhit()) {
//      const int key = getchar();
//      if(key == 0x1b || key == 'q') {
//        break;
//      }
//    }
  }
  mGnss_grid.Finish();
  while(!mGnss_grid.isStopped());
//  }

//  mGnss_grid.Finish();
//  if (!interface.StopStreaming()) {
//    std::cout<<("Failed to stop streaming.  :(\n")<<std::endl;
//    return -1;
//  }
}


