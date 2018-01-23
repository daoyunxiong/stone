#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "main.h"
#include "gnss_sins_ekf_loose_integration_initialize.h"
#include "gnss_sins_ekf_loose_integration_terminate.h"

static e_gnss_sins_ekf_loose_integrati f_gnss_sins_ekf_loose_integrati;
static d_gnss_sins_ekf_loose_integrati g_gnss_sins_ekf_loose_integrati;
static void argInit_1x3_real_T(double result[3]);
static void argInit_1x7_real_T(double result[7]);
static double argInit_real_T(void);
static void c_main_gnss_sins_ekf_loose_inte(void);
static void argInit_1x3_real_T(double result[3])
{
  int idx1;
  for (idx1 = 0; idx1 < 3; idx1++) {
    result[idx1] = argInit_real_T();
  }
}

static void argInit_1x7_real_T(double result[7])
{
  int idx1;
  for (idx1 = 0; idx1 < 7; idx1++) {
    result[idx1] = argInit_real_T();
  }
}

static double argInit_real_T(void)
{
  return 0.0;
}

static void c_main_gnss_sins_ekf_loose_inte(void)
{
  double dv21[7];
  double dv22[3];
  double dv23[3];
  double dv24[3];
  double dv25[3];
  double pos_ins_s_t[3];
  double vel_ins_s_t[3];
  double eul_ins_s_t[3];
  double accelBias_s_t[3];
  double gyroBias_s_t[3];
  double P_t[225];
  double FeedBacks_t[115];
  argInit_1x7_real_T(dv21);
  argInit_1x3_real_T(dv22);
  argInit_1x3_real_T(dv23);
  argInit_1x3_real_T(dv24);
  argInit_1x3_real_T(dv25);
  gnss_sins_ekf_loose_integration(&f_gnss_sins_ekf_loose_integrati,
    argInit_real_T(), dv21, dv22, dv23, dv24, dv25, argInit_real_T(),
    pos_ins_s_t, vel_ins_s_t, eul_ins_s_t, accelBias_s_t, gyroBias_s_t, P_t,
    FeedBacks_t);
}

int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;
  f_gnss_sins_ekf_loose_integrati.pd = &g_gnss_sins_ekf_loose_integrati;
  gnss_sins_ekf_loose_integration_initialize(&f_gnss_sins_ekf_loose_integrati);
  c_main_gnss_sins_ekf_loose_inte();
  gnss_sins_ekf_loose_integration_terminate();
  return 0;
}
