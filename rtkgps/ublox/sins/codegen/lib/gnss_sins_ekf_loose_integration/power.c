#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "power.h"
#include "expm.h"
#include "gnss_sins_ekf_loose_integration_rtwutil.h"

void b_power(const double a[3], double y[3])
{
  int k;
  for (k = 0; k < 3; k++) {
    y[k] = rt_powd_snf(a[k], 2.0);
  }
}

void power(const double a[15], double y[15])
{
  int k;
  for (k = 0; k < 15; k++) {
    y[k] = a[k] * a[k];
  }
}
