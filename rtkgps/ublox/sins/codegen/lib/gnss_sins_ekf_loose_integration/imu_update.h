#ifndef IMU_UPDATE_H
#define IMU_UPDATE_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_sins_ekf_loose_integration_types.h"

extern void G_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void I3_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void Rw_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void b_sigma_a_d_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void b_sigma_g_d_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void imu_update(e_gnss_sins_ekf_loose_integrati *SD, double tau, const
  double imus[7], const double pos_ins[3], const double vel_ins[3], const double
  eul_ins[3], const double accelBias[3], const double gyroBias[3], const double
  P[225], double pos_ins_t[3], double vel_ins_t[3], double eul_ins_t[3], double
  P_t[225], double *R_N, double *R_E);
extern void imu_update_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void sigma_a_w_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void sigma_g_w_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);

#endif
