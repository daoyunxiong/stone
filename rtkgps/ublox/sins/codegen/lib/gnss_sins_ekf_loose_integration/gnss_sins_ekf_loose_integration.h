#ifndef GNSS_SINS_EKF_LOOSE_INTEGRATION_H
#define GNSS_SINS_EKF_LOOSE_INTEGRATION_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_sins_ekf_loose_integration_types.h"

extern void P_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void accelBias_s_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void c_gnss_sins_ekf_loose_integrati(e_gnss_sins_ekf_loose_integrati *SD);
extern void gnss_sins_ekf_loose_integration(e_gnss_sins_ekf_loose_integrati *SD,
  double tau, const double imus[7], const double gps_pos_llas[3], const double
  gps_vel_neds[3], const double initial_accel_bias[3], const double
  initial_gyro_bias[3], double flag, double pos_ins_s_t[3], double vel_ins_s_t[3],
  double eul_ins_s_t[3], double accelBias_s_t[3], double gyroBias_s_t[3], double
  P_t[225], double FeedBacks_t[115]);
extern void gyroBias_s_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void pos_ins_s_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void r2d_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void sigma_a_d_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void sigma_g_d_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void vel_ins_s_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);

#endif
