#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "imu_update.h"
#include "gnss_update.h"
#include "mean.h"
#include "var.h"
#include "mod.h"
#include "norm.h"
#include "diag.h"
#include "power.h"

void P_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->P_not_empty = false;
}

void accelBias_s_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->accelBias_s_not_empty = false;
}

void c_gnss_sins_ekf_loose_integrati(e_gnss_sins_ekf_loose_integrati *SD)
{
  int i15;
  SD->pd->d2r = 0.017453292519943295;
  SD->pd->g = 9.81;
  SD->pd->gps_pos_sigma = 3.0;
  SD->pd->gps_vel_sigma = 0.2;
  for (i15 = 0; i15 < 3; i15++) {
    SD->pd->eul_ins_s[i15] = 0.0;
  }

  SD->pd->buf_len = 50.0;
  memset(&SD->pd->buf[0], 0, 400U * sizeof(double));
  SD->pd->buf_index = 1.0;
}

void gnss_sins_ekf_loose_integration(e_gnss_sins_ekf_loose_integrati *SD, double
  tau, const double imus[7], const double gps_pos_llas[3], const double
  gps_vel_neds[3], const double initial_accel_bias[3], const double
  initial_gyro_bias[3], double flag, double pos_ins_s_t[3], double vel_ins_s_t[3],
  double eul_ins_s_t[3], double accelBias_s_t[3], double gyroBias_s_t[3], double
  P_t[225], double FeedBacks_t[115])
{
  double a;
  double b_a;
  double c_a;
  int i;
  double dv0[15];
  double dv1[15];
  double P[225];
  double FeedBacks[15];
  int i0;
  double buf_vars[50];
  double buf_means[50];
  double eul_ins_s1[3];
  double pos_ins_s[3];
  double vel_ins_s[3];
  double eul_ins_s[3];
  double R_N;
  double R_E;
  double gyroBias_s[3];
  if (!SD->pd->r2d_not_empty) {
    SD->pd->r2d_not_empty = true;
  }

  if (!SD->pd->sigma_g_d_not_empty) {
    SD->pd->sigma_g_d = 0.3 * SD->pd->d2r;
    SD->pd->sigma_g_d_not_empty = true;
  }

  if (!SD->pd->sigma_a_d_not_empty) {
    SD->pd->sigma_a_d = 0.0005 * SD->pd->g;
    SD->pd->sigma_a_d_not_empty = true;
  }

  if (!SD->pd->P_not_empty) {
    a = 10.0 * SD->pd->d2r;
    b_a = 10.0 * SD->pd->sigma_a_d;
    c_a = 10.0 * SD->pd->sigma_g_d;
    for (i = 0; i < 3; i++) {
      dv0[i] = SD->pd->gps_pos_sigma;
      dv0[i + 3] = SD->pd->gps_pos_sigma;
      dv0[i + 6] = a;
      dv0[i + 9] = 10.0 * b_a;
      dv0[i + 12] = c_a;
    }

    power(dv0, dv1);
    diag(dv1, P);
    for (i = 0; i < 15; i++) {
      for (i0 = 0; i0 < 15; i0++) {
        SD->pd->P[i0 + 15 * i] = P[i + 15 * i0];
      }
    }

    for (i = 0; i < 225; i++) {
      SD->pd->P[i] *= 10.0;
    }

    SD->pd->P_not_empty = true;
  }

  if (!SD->pd->pos_ins_s_not_empty) {
    for (i = 0; i < 3; i++) {
      SD->pd->pos_ins_s[i] = gps_pos_llas[i];
    }

    SD->pd->pos_ins_s_not_empty = true;
  }

  if (!SD->pd->vel_ins_s_not_empty) {
    for (i = 0; i < 3; i++) {
      SD->pd->vel_ins_s[i] = gps_vel_neds[i];
    }

    SD->pd->vel_ins_s_not_empty = true;
  }

  if (!SD->pd->accelBias_s_not_empty) {
    for (i = 0; i < 3; i++) {
      SD->pd->accelBias_s[i] = initial_accel_bias[i];
    }

    SD->pd->accelBias_s_not_empty = true;
  }

  if (!SD->pd->gyroBias_s_not_empty) {
    for (i = 0; i < 3; i++) {
      SD->pd->gyroBias_s[i] = initial_gyro_bias[i];
    }

    SD->pd->gyroBias_s_not_empty = true;
  }

  memset(&FeedBacks[0], 0, 15U * sizeof(double));
  for (i = 0; i < 6; i++) {
    SD->pd->buf[((int)SD->pd->buf_index + 50 * i) - 1] = imus[1 + i];
  }

  SD->pd->buf[(int)SD->pd->buf_index + 299] = norm(*(double (*)[3])&imus[1]);
  SD->pd->buf[(int)SD->pd->buf_index + 349] = norm(*(double (*)[3])&imus[4]);
  SD->pd->buf_index = b_mod(SD->pd->buf_index, SD->pd->buf_len) + 1.0;
  var(SD->pd->buf, buf_vars);
  mean(SD->pd->buf, buf_means);
  for (i = 0; i < 3; i++) {
    eul_ins_s1[i] = SD->pd->eul_ins_s[i];
  }

  imu_update(SD, tau, imus, SD->pd->pos_ins_s, SD->pd->vel_ins_s, SD->
             pd->eul_ins_s, SD->pd->accelBias_s, SD->pd->gyroBias_s, SD->pd->P,
             pos_ins_s, vel_ins_s, eul_ins_s, P, &R_N, &R_E);
  for (i = 0; i < 3; i++) {
    SD->pd->pos_ins_s[i] = pos_ins_s[i];
    SD->pd->vel_ins_s[i] = vel_ins_s[i];
    SD->pd->eul_ins_s[i] = eul_ins_s[i];
  }

  memcpy(&SD->pd->P[0], &P[0], 225U * sizeof(double));
  if (flag != 0.0) {
    gnss_update(SD, gps_pos_llas, gps_vel_neds, SD->pd->pos_ins_s, SD->
                pd->vel_ins_s, eul_ins_s1, SD->pd->accelBias_s, SD->
                pd->gyroBias_s, SD->pd->P, R_N, R_E, SD->pd->gps_pos_sigma,
                SD->pd->gps_vel_sigma, pos_ins_s, vel_ins_s, SD->pd->eul_ins_s,
                eul_ins_s, gyroBias_s, P, FeedBacks);
    for (i = 0; i < 3; i++) {
      SD->pd->pos_ins_s[i] = pos_ins_s[i];
      SD->pd->vel_ins_s[i] = vel_ins_s[i];
      SD->pd->accelBias_s[i] = eul_ins_s[i];
      SD->pd->gyroBias_s[i] = gyroBias_s[i];
    }

    memcpy(&SD->pd->P[0], &P[0], 225U * sizeof(double));
  }

  for (i = 0; i < 3; i++) {
    pos_ins_s_t[i] = SD->pd->pos_ins_s[i];
    vel_ins_s_t[i] = SD->pd->vel_ins_s[i];
    eul_ins_s_t[i] = SD->pd->eul_ins_s[i];
    accelBias_s_t[i] = SD->pd->accelBias_s[i];
    gyroBias_s_t[i] = SD->pd->gyroBias_s[i];
  }

  memcpy(&P_t[0], &SD->pd->P[0], 225U * sizeof(double));
  memcpy(&FeedBacks_t[0], &FeedBacks[0], 15U * sizeof(double));
  for (i = 0; i < 50; i++) {
    FeedBacks_t[i + 15] = buf_vars[i];
    FeedBacks_t[i + 65] = buf_means[i];
  }
}

void gyroBias_s_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->gyroBias_s_not_empty = false;
}

void pos_ins_s_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->pos_ins_s_not_empty = false;
}

void r2d_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->r2d_not_empty = false;
}

void sigma_a_d_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->sigma_a_d_not_empty = false;
}

void sigma_g_d_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->sigma_g_d_not_empty = false;
}

void vel_ins_s_not_empty_init(e_gnss_sins_ekf_loose_integrati *SD)
{
  SD->pd->vel_ins_s_not_empty = false;
}
