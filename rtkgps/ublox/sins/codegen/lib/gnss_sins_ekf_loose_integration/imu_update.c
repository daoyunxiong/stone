#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "imu_update.h"
#include "discrete_process_noise.h"
#include "expm.h"
#include "diag.h"
#include "norm.h"
#include "q2eul.h"
#include "qupdt.h"
#include "eul2q.h"
#include "earthrad.h"
#include "cross.h"
#include "glocal.h"
#include "navrate.h"
#include "eul2Cbn.h"
#include "eye.h"

void G_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->G_not_empty = false;
}

void I3_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->I3_not_empty = false;
}

void Rw_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->Rw_not_empty = false;
}

void b_sigma_a_d_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->b_sigma_a_d_not_empty = false;
}

void b_sigma_g_d_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->b_sigma_g_d_not_empty = false;
}

void imu_update(e_gnss_sins_ekf_loose_integrati *SD, double tau, const double
                imus[7], const double pos_ins[3], const double vel_ins[3], const
                double eul_ins[3], const double accelBias[3], const double
                gyroBias[3], const double P[225], double pos_ins_t[3], double
                vel_ins_t[3], double eul_ins_t[3], double P_t[225], double *R_N,
                double *R_E)
{
  int i1;
  int i;
  double Cbn[9];
  double b_tau[3];
  double c;
  double b_c;
  double omega_n_en[3];
  double y;
  double omega_n_ie[3];
  double b_y;
  double f_b[3];
  double g_n[3];
  static const signed char b[9] = { 1, 0, 0, 0, 1, 0, 0, 0, 1 };

  double v_dot[3];
  double c_y[3];
  double T[9];
  double qnb[4];
  double b_imus[3];
  double c_imus[3];
  double b_qnb[4];
  double dv2[4];
  double g_mag;
  static const signed char b_b[3] = { -1, -1, 2 };

  double dpsi2dv[9];
  double dv3[9];
  double dpsi2dpsi[9];
  double domega2dpsi[9];
  double domega2domega[9];
  double dv4[3];
  double dv5[9];
  double F[225];
  double b_F[225];
  double dv6[9];
  double PHI[225];
  double Q_k[225];
  int i2;
  if (!SD->pd->sigma_g_w_not_empty) {
    SD->pd->sigma_g_w = 0.95 * SD->pd->b_d2r;
    SD->pd->sigma_g_w_not_empty = true;
  }

  if (!SD->pd->sigma_a_w_not_empty) {
    SD->pd->sigma_a_w = 0.12 * SD->pd->b_g;
    SD->pd->sigma_a_w_not_empty = true;
  }

  if (!SD->pd->b_sigma_g_d_not_empty) {
    SD->pd->b_sigma_g_d = 0.3 * SD->pd->b_d2r;
    SD->pd->b_sigma_g_d_not_empty = true;
  }

  if (!SD->pd->b_sigma_a_d_not_empty) {
    SD->pd->b_sigma_a_d = 0.0005 * SD->pd->b_g;
    SD->pd->b_sigma_a_d_not_empty = true;
  }

  if (!SD->pd->I3_not_empty) {
    eye(SD->pd->I3);
    SD->pd->I3_not_empty = true;
  }

  if (!SD->pd->G_not_empty) {
    for (i1 = 0; i1 < 3; i1++) {
      for (i = 0; i < 3; i++) {
        SD->pd->G[i + 15 * i1] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[i + 15 * (i1 + 3)] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[i + 15 * (i1 + 6)] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[i + 15 * (i1 + 9)] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * i1) + 3] = SD->pd->I3[i + 3 * i1];
        SD->pd->G[(i + 15 * (i1 + 3)) + 3] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * (i1 + 6)) + 3] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * (i1 + 9)) + 3] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * i1) + 6] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * (i1 + 3)) + 6] = SD->pd->I3[i + 3 * i1];
        SD->pd->G[(i + 15 * (i1 + 6)) + 6] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * (i1 + 9)) + 6] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * i1) + 9] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * (i1 + 3)) + 9] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * (i1 + 6)) + 9] = SD->pd->I3[i + 3 * i1];
        SD->pd->G[(i + 15 * (i1 + 9)) + 9] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * i1) + 12] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * (i1 + 3)) + 12] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * (i1 + 6)) + 12] = SD->pd->Z3[i + 3 * i1];
        SD->pd->G[(i + 15 * (i1 + 9)) + 12] = SD->pd->I3[i + 3 * i1];
      }
    }

    SD->pd->G_not_empty = true;
  }

  if (!SD->pd->Rw_not_empty) {
    memset(&SD->pd->Rw[0], 0, 144U * sizeof(double));
    SD->pd->Rw_not_empty = true;
    c = SD->pd->sigma_a_w * SD->pd->sigma_a_w;
    b_c = SD->pd->sigma_g_w * SD->pd->sigma_g_w;
    y = 2.0 * (SD->pd->b_sigma_a_d * SD->pd->b_sigma_a_d) / SD->pd->tau_a;
    b_y = 2.0 * (SD->pd->b_sigma_g_d * SD->pd->b_sigma_g_d) / SD->pd->tau_g;
    for (i1 = 0; i1 < 3; i1++) {
      for (i = 0; i < 3; i++) {
        SD->pd->Rw[i + 12 * i1] = c * (double)b[i + 3 * i1];
        SD->pd->Rw[(i + 12 * (3 + i1)) + 3] = b_c * (double)b[i + 3 * i1];
        SD->pd->Rw[(i + 12 * (6 + i1)) + 6] = y * (double)b[i + 3 * i1];
        SD->pd->Rw[(i + 12 * (9 + i1)) + 9] = b_y * (double)b[i + 3 * i1];
      }
    }
  }

  eul2Cbn(eul_ins, Cbn);
  b_tau[0] = cos(pos_ins[0]);
  b_tau[1] = 0.0;
  b_tau[2] = -sin(pos_ins[0]);
  navrate(vel_ins, pos_ins, omega_n_en);
  for (i1 = 0; i1 < 3; i1++) {
    omega_n_ie[i1] = 7.292115E-5 * b_tau[i1];
    f_b[i1] = imus[4 + i1] + accelBias[i1];
  }

  glocal(pos_ins[0], pos_ins[2], g_n);
  if (SD->pd->NO_CORIOLIS != 0.0) {
    for (i1 = 0; i1 < 3; i1++) {
      c = 0.0;
      for (i = 0; i < 3; i++) {
        c += Cbn[i1 + 3 * i] * f_b[i];
      }

      v_dot[i1] = c + g_n[i1];
    }
  } else {
    for (i = 0; i < 3; i++) {
      b_tau[i] = 2.0 * omega_n_ie[i] + omega_n_en[i];
    }

    cross(b_tau, vel_ins, c_y);
    for (i1 = 0; i1 < 3; i1++) {
      c = 0.0;
      for (i = 0; i < 3; i++) {
        c += Cbn[i1 + 3 * i] * f_b[i];
      }

      v_dot[i1] = (c - c_y[i1]) + g_n[i1];
    }
  }

  for (i1 = 0; i1 < 3; i1++) {
    vel_ins_t[i1] = vel_ins[i1] + tau * v_dot[i1];
  }

  earthrad(pos_ins[0], R_N, R_E);
  memset(&T[0], 0, 9U * sizeof(double));
  T[0] = 1.0 / (*R_N + pos_ins[2]);
  T[4] = 1.0 / ((*R_E + pos_ins[2]) * cos(pos_ins[0]));
  T[8] = -1.0;
  for (i1 = 0; i1 < 3; i1++) {
    b_tau[i1] = 0.0;
    for (i = 0; i < 3; i++) {
      b_tau[i1] += tau * T[i1 + 3 * i] * vel_ins[i];
    }

    pos_ins_t[i1] = pos_ins[i1] + b_tau[i1];
  }

  vel_ins_t[2] = vel_ins[2];
  pos_ins_t[2] = pos_ins[2];
  eul2q(eul_ins, qnb);
  for (i1 = 0; i1 < 3; i1++) {
    b_tau[i1] = omega_n_en[i1] + omega_n_ie[i1];
  }

  for (i1 = 0; i1 < 3; i1++) {
    c = 0.0;
    for (i = 0; i < 3; i++) {
      c += Cbn[i1 + 3 * i] * b_tau[i];
    }

    b_imus[i1] = (imus[1 + i1] - c) + gyroBias[i1];
    c_imus[i1] = b_imus[i1] * tau;
  }

  for (i1 = 0; i1 < 4; i1++) {
    b_qnb[i1] = qnb[i1];
  }

  qupdt(b_qnb, c_imus, dv2);
  q2eul(dv2, eul_ins_t);
  g_mag = b_norm(g_n);
  y = g_mag / SD->pd->R_0;
  for (i = 0; i < 3; i++) {
    b_tau[i] = y * (double)b_b[i];
    g_n[i] = 2.0 * omega_n_ie[i] + omega_n_en[i];
    c_y[i] = 0.0;
    for (i1 = 0; i1 < 3; i1++) {
      c_y[i] += Cbn[i + 3 * i1] * f_b[i1];
    }

    omega_n_ie[i] += omega_n_en[i];
  }

  b_diag(b_tau, T);
  dpsi2dv[0] = 0.0;
  dpsi2dv[3] = -c_y[2];
  dpsi2dv[6] = c_y[1];
  dpsi2dv[1] = c_y[2];
  dpsi2dv[4] = 0.0;
  dpsi2dv[7] = -c_y[0];
  dpsi2dv[2] = -c_y[1];
  dpsi2dv[5] = c_y[0];
  dpsi2dv[8] = 0.0;
  dv3[0] = 0.0;
  dv3[3] = -omega_n_ie[2];
  dv3[6] = omega_n_ie[1];
  dv3[1] = omega_n_ie[2];
  dv3[4] = 0.0;
  dv3[7] = -omega_n_ie[0];
  dv3[2] = -omega_n_ie[1];
  dv3[5] = omega_n_ie[0];
  dv3[8] = 0.0;
  for (i1 = 0; i1 < 3; i1++) {
    for (i = 0; i < 3; i++) {
      dpsi2dpsi[i + 3 * i1] = -dv3[i + 3 * i1];
    }
  }

  for (i1 = 0; i1 < 9; i1++) {
    domega2dpsi[i1] = -Cbn[i1];
    domega2domega[i1] = -SD->pd->I3[i1] / SD->pd->tau_g;
  }

  if (!(SD->pd->SMALL_PROP_TIME != 0.0)) {
    dv5[0] = 0.0;
    dv5[3] = -omega_n_en[2];
    dv5[6] = omega_n_en[1];
    dv5[1] = omega_n_en[2];
    dv5[4] = 0.0;
    dv5[7] = -omega_n_en[0];
    dv5[2] = -omega_n_en[1];
    dv5[5] = omega_n_en[0];
    dv5[8] = 0.0;
    dv6[0] = 0.0;
    dv6[3] = -g_n[2];
    dv6[6] = g_n[1];
    dv6[1] = g_n[2];
    dv6[4] = 0.0;
    dv6[7] = -g_n[0];
    dv6[2] = -g_n[1];
    dv6[5] = g_n[0];
    dv6[8] = 0.0;
    for (i1 = 0; i1 < 3; i1++) {
      for (i = 0; i < 3; i++) {
        F[i + 15 * i1] = -dv5[i + 3 * i1];
        F[i + 15 * (i1 + 3)] = SD->pd->I3[i + 3 * i1];
        F[i + 15 * (i1 + 6)] = SD->pd->Z3[i + 3 * i1];
        F[i + 15 * (i1 + 9)] = SD->pd->Z3[i + 3 * i1];
        F[i + 15 * (i1 + 12)] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * i1) + 3] = T[i + 3 * i1];
        F[(i + 15 * (i1 + 3)) + 3] = -dv6[i + 3 * i1];
        F[(i + 15 * (i1 + 6)) + 3] = dpsi2dv[i + 3 * i1];
        F[(i + 15 * (i1 + 9)) + 3] = Cbn[i + 3 * i1];
        F[(i + 15 * (i1 + 12)) + 3] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * i1) + 6] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 3)) + 6] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 6)) + 6] = dpsi2dpsi[i + 3 * i1];
        F[(i + 15 * (i1 + 9)) + 6] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 12)) + 6] = domega2dpsi[i + 3 * i1];
        F[(i + 15 * i1) + 9] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 3)) + 9] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 6)) + 9] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 9)) + 9] = -SD->pd->I3[i + 3 * i1] / SD->pd->tau_a;
        F[(i + 15 * (i1 + 12)) + 9] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * i1) + 12] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 3)) + 12] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 6)) + 12] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 9)) + 12] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 12)) + 12] = domega2domega[i + 3 * i1];
      }
    }
  } else {
    dv4[0] = 0.0;
    dv4[1] = 0.0;
    dv4[2] = 2.0 * g_mag / SD->pd->R_0;
    b_diag(dv4, T);
    for (i1 = 0; i1 < 3; i1++) {
      for (i = 0; i < 3; i++) {
        F[i + 15 * i1] = SD->pd->Z3[i + 3 * i1];
        F[i + 15 * (i1 + 3)] = SD->pd->I3[i + 3 * i1];
        F[i + 15 * (i1 + 6)] = SD->pd->Z3[i + 3 * i1];
        F[i + 15 * (i1 + 9)] = SD->pd->Z3[i + 3 * i1];
        F[i + 15 * (i1 + 12)] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * i1) + 3] = T[i + 3 * i1];
        F[(i + 15 * (i1 + 3)) + 3] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 6)) + 3] = dpsi2dv[i + 3 * i1];
        F[(i + 15 * (i1 + 9)) + 3] = Cbn[i + 3 * i1];
        F[(i + 15 * (i1 + 12)) + 3] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * i1) + 6] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 3)) + 6] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 6)) + 6] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 9)) + 6] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 12)) + 6] = domega2dpsi[i + 3 * i1];
        F[(i + 15 * i1) + 9] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 3)) + 9] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 6)) + 9] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 9)) + 9] = dpsi2dpsi[i + 3 * i1];
        F[(i + 15 * (i1 + 12)) + 9] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * i1) + 12] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 3)) + 12] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 6)) + 12] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 9)) + 12] = SD->pd->Z3[i + 3 * i1];
        F[(i + 15 * (i1 + 12)) + 12] = domega2domega[i + 3 * i1];
      }
    }
  }

  for (i1 = 0; i1 < 3; i1++) {
    for (i = 0; i < 3; i++) {
      SD->pd->G[(i + 15 * i1) + 3] = Cbn[i + 3 * i1];
      SD->pd->G[(i + 15 * (3 + i1)) + 6] = -Cbn[i + 3 * i1];
    }
  }

  for (i1 = 0; i1 < 225; i1++) {
    b_F[i1] = F[i1] * tau;
  }

  expm(b_F, PHI);
  discrete_process_noise(F, SD->pd->G, tau, SD->pd->Rw, Q_k);
  for (i1 = 0; i1 < 15; i1++) {
    for (i = 0; i < 15; i++) {
      b_F[i1 + 15 * i] = 0.0;
      for (i2 = 0; i2 < 15; i2++) {
        b_F[i1 + 15 * i] += PHI[i1 + 15 * i2] * P[i2 + 15 * i];
      }
    }

    for (i = 0; i < 15; i++) {
      c = 0.0;
      for (i2 = 0; i2 < 15; i2++) {
        c += b_F[i1 + 15 * i2] * PHI[i + 15 * i2];
      }

      P_t[i1 + 15 * i] = c + Q_k[i1 + 15 * i];
    }
  }
}

void imu_update_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->NO_CORIOLIS = 1.0;
  SD->pd->SMALL_PROP_TIME = 1.0;
  SD->pd->R_0 = 6.378137E+6;
  SD->pd->b_g = 9.81;
  SD->pd->b_d2r = 0.017453292519943295;
  SD->pd->tau_g = 300.0;
  SD->pd->tau_a = 300.0;
  memset(&SD->pd->Z3[0], 0, 9U * sizeof(double));
}

void sigma_a_w_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->sigma_a_w_not_empty = false;
}

void sigma_g_w_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->sigma_g_w_not_empty = false;
}
