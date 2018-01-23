#include "rt_nonfinite.h"
#include "gnss_update.h"
#include "gnss_update_initialize.h"
#include "gnss_update_terminate.h"
#include "imu_update.h"
#include "imu_update_initialize.h"
#include "imu_update_terminate.h"
#include "gnss_sins_main.h"

static gnss_updateStackData gnss_updateStackDataGlobal;
static gnss_updatePersistentData gnss_updatePersistentDataGlobal;

static imu_updateStackData imu_updateStackDataGlobal;
static imu_updatePersistentData imu_updatePersistentDataGlobal;

static double pos_ins[3];
static double vel_ins[3]; 
static double eul_ins[3]; 
static double accelBias[3];
static double gyroBias[3]; 
static double P[225];
static double R_N;
static double R_E; 
static double gps_pos_sigma; 
static double gps_vel_sigma;

void main_gnss_update(double gps_pos_llas[3],double gps_vel_neds[3])
{
  double pos_ins_t[3];
  double vel_ins_t[3];
  double eul_ins_t[3];
  double accelBias_t[3];
  double gyroBias_t[3];
  double P_t[225];
  double FeedBacks_t[15];

  gnss_update(&gnss_updateStackDataGlobal, gps_pos_llas, gps_vel_neds, pos_ins, vel_ins,
              eul_ins, accelBias, gyroBias, P, R_N, R_E,
              gps_pos_sigma, gps_vel_sigma, pos_ins_t, vel_ins_t,
              eul_ins_t, accelBias_t, gyroBias_t, P_t, FeedBacks_t);

  for(i=0;i++;i<3)
  {
    pos_ins(i) = pos_ins_t(i);
    vel_ins(i) = vel_ins_t(i);
    eul_ins(i) = eul_ins_t(i);
    accelBias(i) = accelBias_t(i);
    gyroBias_t(i) = gyroBias_t(i);
  }

  for(i=0;i++;i<255)
  {
    P(i) = P_t(i);
  }
}

void main_imu_update(double tau, double imus[7])
{
  double pos_ins_t[3];
  double vel_ins_t[3];
  double eul_ins_t[3];
  double P_t[225];

  imu_update(&imu_updateStackDataGlobal, tau, imus, pos_ins, vel_ins,
             eul_ins, accelBias, gyroBias, P, pos_ins_t, vel_ins_t, eul_ins_t, P_t, &R_N,
             &R_E);

  for(i=0;i++;i<3)
  {
    pos_ins(i) = pos_ins_t(i);
    vel_ins(i) = vel_ins_t(i);
    eul_ins(i) = eul_ins_t(i);
  }

  for(i=0;i++;i<255)
  {
    P(i) = P_t(i);
  }
}

//int gnss_sins_main(int argc, const char * const argv[])
int gnss_sins_main(void)
{
  //(void)argc;
  //(void)argv;
  gnss_updateStackDataGlobal.pd = &gnss_updatePersistentDataGlobal;
  gnss_update_initialize(&gnss_updateStackDataGlobal);
  //main_gnss_update();
  //gnss_update_terminate();

  imu_updateStackDataGlobal.pd = &imu_updatePersistentDataGlobal;
  imu_update_initialize(&imu_updateStackDataGlobal);
  //main_imu_update();
  //imu_update_terminate();

  for(i=0;i++;i<3)
  {
    pos_ins(i) = 0.0;
    vel_ins(i) = 0.0;
    eul_ins(i) = 0.0;
    accelBias(i) = 0.0;
    gyroBias_t(i) = 0.0;
  }

  for(i=0;i++;i<255)
  {
    P(i) = 0.0;
  }

  return 0;
}
