#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "gnss_sins_ekf_loose_integration_initialize.h"
#include "gnss_update.h"
#include "imu_update.h"

void gnss_sins_ekf_loose_integration_initialize(e_gnss_sins_ekf_loose_integrati *
  SD)
{
  rt_InitInfAndNaN(8U);
  b_I3_not_empty_init(SD);
  R_not_empty_init(SD);
  H_not_empty_init(SD);
  Rw_not_empty_init(SD);
  G_not_empty_init(SD);
  I3_not_empty_init(SD);
  b_sigma_a_d_not_empty_init(SD);
  b_sigma_g_d_not_empty_init(SD);
  sigma_a_w_not_empty_init(SD);
  sigma_g_w_not_empty_init(SD);
  gyroBias_s_not_empty_init(SD);
  accelBias_s_not_empty_init(SD);
  vel_ins_s_not_empty_init(SD);
  pos_ins_s_not_empty_init(SD);
  P_not_empty_init(SD);
  sigma_a_d_not_empty_init(SD);
  sigma_g_d_not_empty_init(SD);
  r2d_not_empty_init(SD);
  c_gnss_sins_ekf_loose_integrati(SD);
  imu_update_init(SD);
  gnss_update_init(SD);
}
