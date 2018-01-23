#ifndef GNSS_SINS_EKF_LOOSE_INTEGRATION_TYPES_H
#define GNSS_SINS_EKF_LOOSE_INTEGRATION_TYPES_H
#include "rtwtypes.h"
#ifndef typedef_d_gnss_sins_ekf_loose_integrati
#define typedef_d_gnss_sins_ekf_loose_integrati

typedef struct {
  double d2r;
  boolean_T r2d_not_empty;
  double g;
  double sigma_g_d;
  boolean_T sigma_g_d_not_empty;
  double sigma_a_d;
  boolean_T sigma_a_d_not_empty;
  double gps_pos_sigma;
  double gps_vel_sigma;
  double P[225];
  boolean_T P_not_empty;
  double pos_ins_s[3];
  boolean_T pos_ins_s_not_empty;
  double vel_ins_s[3];
  boolean_T vel_ins_s_not_empty;
  double accelBias_s[3];
  boolean_T accelBias_s_not_empty;
  double gyroBias_s[3];
  boolean_T gyroBias_s_not_empty;
  double eul_ins_s[3];
  double buf_len;
  double buf[400];
  double buf_index;
  double NO_CORIOLIS;
  double SMALL_PROP_TIME;
  double R_0;
  double b_g;
  double b_d2r;
  double tau_g;
  double tau_a;
  double sigma_g_w;
  boolean_T sigma_g_w_not_empty;
  double sigma_a_w;
  boolean_T sigma_a_w_not_empty;
  double b_sigma_g_d;
  boolean_T b_sigma_g_d_not_empty;
  double b_sigma_a_d;
  boolean_T b_sigma_a_d_not_empty;
  double I3[9];
  boolean_T I3_not_empty;
  double Z3[9];
  double G[180];
  boolean_T G_not_empty;
  double Rw[144];
  boolean_T Rw_not_empty;
  double r2d;
  double H[90];
  boolean_T H_not_empty;
  double R[36];
  boolean_T R_not_empty;
  double b_I3[9];
  boolean_T b_I3_not_empty;
} d_gnss_sins_ekf_loose_integrati;

#endif

#ifndef typedef_e_gnss_sins_ekf_loose_integrati
#define typedef_e_gnss_sins_ekf_loose_integrati

typedef struct {
  d_gnss_sins_ekf_loose_integrati *pd;
} e_gnss_sins_ekf_loose_integrati;

#endif
#endif
