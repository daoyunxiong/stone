#ifndef _CODER_GNSS_SINS_EKF_LOOSE_INTEGRATION_API_H
#define _CODER_GNSS_SINS_EKF_LOOSE_INTEGRATION_API_H
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_gnss_sins_ekf_loose_integration_api.h"

extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;
extern void gnss_sins_ekf_loose_integration(real_T tau, real_T imus[7], real_T
  gps_pos_llas[3], real_T gps_vel_neds[3], real_T initial_accel_bias[3], real_T
  initial_gyro_bias[3], real_T flag, real_T pos_ins_s_t[3], real_T vel_ins_s_t[3],
  real_T eul_ins_s_t[3], real_T accelBias_s_t[3], real_T gyroBias_s_t[3], real_T
  P_t[225], real_T FeedBacks_t[115]);
extern void gnss_sins_ekf_loose_integration_api(const mxArray *prhs[7], const
  mxArray *plhs[7]);
extern void gnss_sins_ekf_loose_integration_atexit(void);
extern void gnss_sins_ekf_loose_integration_initialize(void);
extern void gnss_sins_ekf_loose_integration_terminate(void);
extern void gnss_sins_ekf_loose_integration_xil_terminate(void);

#endif
