#ifndef GNSS_UPDATE_H
#define GNSS_UPDATE_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_sins_ekf_loose_integration_types.h"

extern void H_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void R_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void b_I3_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD);
extern void gnss_update(e_gnss_sins_ekf_loose_integrati *SD, const double
  gps_pos_llas[3], const double gps_vel_neds[3], const double pos_ins[3], const
  double vel_ins[3], const double eul_ins[3], const double accelBias[3], const
  double gyroBias[3], const double P[225], double R_N, double R_E, double
  gps_pos_sigma, double gps_vel_sigma, double pos_ins_t[3], double vel_ins_t[3],
  double eul_ins_t[3], double accelBias_t[3], double gyroBias_t[3], double P_t
  [225], double FeedBacks_t[15]);
extern void gnss_update_init(e_gnss_sins_ekf_loose_integrati *SD);

#endif
