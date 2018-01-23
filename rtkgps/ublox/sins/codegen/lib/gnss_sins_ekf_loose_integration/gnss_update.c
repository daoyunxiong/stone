#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "gnss_update.h"
#include "q2eul.h"
#include "wgsxyz2ned.h"
#include "mrdivide.h"
#include "sind.h"
#include "cosd.h"
#include "power.h"
#include "gnss_sins_ekf_loose_integration_rtwutil.h"

void H_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->H_not_empty = false;
}

void R_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->R_not_empty = false;
}

void b_I3_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->b_I3_not_empty = false;
}

void gnss_update(e_gnss_sins_ekf_loose_integrati *SD, const double gps_pos_llas
                 [3], const double gps_vel_neds[3], const double pos_ins[3],
                 const double vel_ins[3], const double eul_ins[3], const double
                 accelBias[3], const double gyroBias[3], const double P[225],
                 double R_N, double R_E, double gps_pos_sigma, double
                 gps_vel_sigma, double pos_ins_t[3], double vel_ins_t[3], double
                 eul_ins_t[3], double accelBias_t[3], double gyroBias_t[3],
                 double P_t[225], double FeedBacks_t[15])
{
  signed char I[36];
  int k;
  double b_gps_pos_sigma[3];
  double lat_ref;
  double dv12[3];
  double lon_ref;
  double a;
  int i11;
  double b_R_E;
  double v[6];
  double d0;
  double p_e[3];
  double lat_gps;
  double lon_gps;
  double b_p_e[3];
  double dv13[36];
  double dv14[90];
  double K[90];
  int i12;
  signed char b_I[225];
  double c_I[225];
  double Pt[225];
  double dv15[9];
  double dv16[9];
  double dv17[9];
  static const signed char iv3[3] = { 1, 0, 0 };

  static const signed char iv4[3] = { 0, 1, 0 };

  double dv18[9];
  double dv19[9];
  double dv20[9];
  static const signed char iv5[3] = { 0, 0, 1 };

  double PSI_nb[3];
  double C[9];
  if (!SD->pd->H_not_empty) {
    for (k = 0; k < 36; k++) {
      I[k] = 0;
    }

    for (k = 0; k < 6; k++) {
      I[k + 6 * k] = 1;
    }

    for (k = 0; k < 6; k++) {
      for (i11 = 0; i11 < 6; i11++) {
        SD->pd->H[i11 + 6 * k] = I[i11 + 6 * k];
      }
    }

    for (k = 0; k < 9; k++) {
      for (i11 = 0; i11 < 6; i11++) {
        SD->pd->H[i11 + 6 * (k + 6)] = 0.0;
      }
    }

    SD->pd->H_not_empty = true;
  }

  if (!SD->pd->R_not_empty) {
    for (k = 0; k < 3; k++) {
      b_gps_pos_sigma[k] = gps_pos_sigma;
    }

    b_power(b_gps_pos_sigma, dv12);
    for (k = 0; k < 3; k++) {
      b_gps_pos_sigma[k] = gps_vel_sigma;
      v[k] = dv12[k];
    }

    b_power(b_gps_pos_sigma, dv12);
    for (k = 0; k < 3; k++) {
      v[k + 3] = dv12[k];
    }

    memset(&SD->pd->R[0], 0, 36U * sizeof(double));
    for (k = 0; k < 6; k++) {
      SD->pd->R[k + 6 * k] = v[k];
    }

    SD->pd->R_not_empty = true;
  }

  if (!SD->pd->b_I3_not_empty) {
    memset(&SD->pd->b_I3[0], 0, 9U * sizeof(double));
    for (k = 0; k < 3; k++) {
      SD->pd->b_I3[k + 3 * k] = 1.0;
    }

    SD->pd->b_I3_not_empty = true;
  }

  lat_ref = pos_ins[0] * SD->pd->r2d;
  lon_ref = pos_ins[1] * SD->pd->r2d;
  a = lat_ref;
  sind(&a);
  a *= 0.081819190842621486;
  b_R_E = 6.378137E+6 / sqrt(1.0 - a * a);
  a = lat_ref;
  cosd(&a);
  d0 = lon_ref;
  cosd(&d0);
  p_e[0] = (b_R_E + pos_ins[2]) * a * d0;
  a = lat_ref;
  cosd(&a);
  d0 = lon_ref;
  sind(&d0);
  p_e[1] = (b_R_E + pos_ins[2]) * a * d0;
  a = lat_ref;
  sind(&a);
  p_e[2] = (0.99330562000985867 * b_R_E + pos_ins[2]) * a;
  lat_gps = gps_pos_llas[0] * SD->pd->r2d;
  lon_gps = gps_pos_llas[1] * SD->pd->r2d;
  a = lat_gps;
  sind(&a);
  a *= 0.081819190842621486;
  b_R_E = 6.378137E+6 / sqrt(1.0 - a * a);
  a = lat_gps;
  cosd(&a);
  d0 = lon_gps;
  cosd(&d0);
  b_p_e[0] = (b_R_E + gps_pos_llas[2]) * a * d0;
  a = lat_gps;
  cosd(&a);
  d0 = lon_gps;
  sind(&d0);
  b_p_e[1] = (b_R_E + gps_pos_llas[2]) * a * d0;
  a = lat_gps;
  sind(&a);
  b_p_e[2] = (0.99330562000985867 * b_R_E + gps_pos_llas[2]) * a;
  for (k = 0; k < 15; k++) {
    for (i11 = 0; i11 < 6; i11++) {
      K[k + 15 * i11] = 0.0;
      for (i12 = 0; i12 < 15; i12++) {
        K[k + 15 * i11] += P[k + 15 * i12] * SD->pd->H[i11 + 6 * i12];
      }
    }
  }

  for (k = 0; k < 6; k++) {
    for (i11 = 0; i11 < 15; i11++) {
      dv14[k + 6 * i11] = 0.0;
      for (i12 = 0; i12 < 15; i12++) {
        dv14[k + 6 * i11] += SD->pd->H[k + 6 * i12] * P[i12 + 15 * i11];
      }
    }

    for (i11 = 0; i11 < 6; i11++) {
      a = 0.0;
      for (i12 = 0; i12 < 15; i12++) {
        a += dv14[k + 6 * i12] * SD->pd->H[i11 + 6 * i12];
      }

      dv13[k + 6 * i11] = a + SD->pd->R[k + 6 * i11];
    }
  }

  mrdivide(K, dv13);
  memset(&b_I[0], 0, 225U * sizeof(signed char));
  for (k = 0; k < 15; k++) {
    b_I[k + 15 * k] = 1;
  }

  for (k = 0; k < 15; k++) {
    for (i11 = 0; i11 < 15; i11++) {
      a = 0.0;
      for (i12 = 0; i12 < 6; i12++) {
        a += K[k + 15 * i12] * SD->pd->H[i12 + 6 * i11];
      }

      c_I[k + 15 * i11] = (double)b_I[k + 15 * i11] - a;
    }

    for (i11 = 0; i11 < 15; i11++) {
      Pt[k + 15 * i11] = 0.0;
      for (i12 = 0; i12 < 15; i12++) {
        Pt[k + 15 * i11] += c_I[k + 15 * i12] * P[i12 + 15 * i11];
      }
    }
  }

  for (k = 0; k < 15; k++) {
    for (i11 = 0; i11 < 15; i11++) {
      P_t[i11 + 15 * k] = 0.5 * (Pt[i11 + 15 * k] + Pt[k + 15 * i11]);
    }
  }

  wgsxyz2ned(p_e, lat_ref, lon_ref, 0.0 * pos_ins[2], dv12);
  wgsxyz2ned(b_p_e, lat_ref, lon_ref, 0.0 * pos_ins[2], p_e);
  for (k = 0; k < 3; k++) {
    v[k] = dv12[k] - p_e[k];
    v[k + 3] = vel_ins[k] - gps_vel_neds[k];
  }

  for (k = 0; k < 15; k++) {
    FeedBacks_t[k] = 0.0;
    for (i11 = 0; i11 < 6; i11++) {
      FeedBacks_t[k] += K[k + 15 * i11] * v[i11];
    }
  }

  pos_ins_t[0] = pos_ins[0] - FeedBacks_t[0] / (R_N + pos_ins[2]);
  pos_ins_t[1] = pos_ins[1] - FeedBacks_t[1] / ((R_E + pos_ins[2]) * cos
    (pos_ins[0]));
  pos_ins_t[2] = gps_pos_llas[2];
  for (k = 0; k < 3; k++) {
    vel_ins_t[k] = vel_ins[k] - FeedBacks_t[3 + k];
  }

  vel_ins_t[2] = gps_vel_neds[2];
  dv15[0] = 0.0;
  dv15[3] = -FeedBacks_t[8];
  dv15[6] = FeedBacks_t[7];
  dv15[1] = FeedBacks_t[8];
  dv15[4] = 0.0;
  dv15[7] = -FeedBacks_t[6];
  dv15[2] = -FeedBacks_t[7];
  dv15[5] = FeedBacks_t[6];
  dv15[8] = 0.0;
  dv16[1] = 0.0;
  dv16[4] = cos(eul_ins[0]);
  dv16[7] = sin(eul_ins[0]);
  dv16[2] = 0.0;
  dv16[5] = -sin(eul_ins[0]);
  dv16[8] = cos(eul_ins[0]);
  dv17[0] = cos(eul_ins[1]);
  dv17[3] = 0.0;
  dv17[6] = -sin(eul_ins[1]);
  for (k = 0; k < 3; k++) {
    dv16[3 * k] = iv3[k];
    dv17[1 + 3 * k] = iv4[k];
  }

  dv17[2] = sin(eul_ins[1]);
  dv17[5] = 0.0;
  dv17[8] = cos(eul_ins[1]);
  dv19[0] = cos(eul_ins[2]);
  dv19[3] = sin(eul_ins[2]);
  dv19[6] = 0.0;
  dv19[1] = -sin(eul_ins[2]);
  dv19[4] = cos(eul_ins[2]);
  dv19[7] = 0.0;
  for (k = 0; k < 3; k++) {
    dv19[2 + 3 * k] = iv5[k];
    for (i11 = 0; i11 < 3; i11++) {
      dv18[k + 3 * i11] = 0.0;
      for (i12 = 0; i12 < 3; i12++) {
        dv18[k + 3 * i11] += dv16[k + 3 * i12] * dv17[i12 + 3 * i11];
      }

      dv20[i11 + 3 * k] = SD->pd->b_I3[i11 + 3 * k] + dv15[i11 + 3 * k];
    }
  }

  for (k = 0; k < 3; k++) {
    for (i11 = 0; i11 < 3; i11++) {
      dv15[k + 3 * i11] = 0.0;
      for (i12 = 0; i12 < 3; i12++) {
        dv15[k + 3 * i11] += dv18[i11 + 3 * i12] * dv19[i12 + 3 * k];
      }
    }
  }

  for (k = 0; k < 3; k++) {
    for (i11 = 0; i11 < 3; i11++) {
      C[k + 3 * i11] = 0.0;
      for (i12 = 0; i12 < 3; i12++) {
        C[k + 3 * i11] += dv20[i11 + 3 * i12] * dv15[i12 + 3 * k];
      }
    }
  }

  PSI_nb[0] = rt_atan2d_snf(C[7], C[8]);
  PSI_nb[2] = rt_atan2d_snf(C[3], C[0]);
  PSI_nb[1] = -asin(C[6]);
  for (k = 0; k < 3; k++) {
    eul_ins_t[k] = PSI_nb[k];
    accelBias_t[k] = accelBias[k] - FeedBacks_t[9 + k];
    gyroBias_t[k] = gyroBias[k] - FeedBacks_t[12 + k];
  }
}

void gnss_update_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->r2d = 57.295779513082323;
}
