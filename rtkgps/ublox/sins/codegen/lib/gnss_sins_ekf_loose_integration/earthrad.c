#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "earthrad.h"
#include "expm.h"
#include "gnss_sins_ekf_loose_integration_rtwutil.h"

void earthrad(double L, double *R_N, double *R_E)
{
  double a;
  double k;
  a = 0.081819190842621486 * sin(L);
  k = sqrt(1.0 - a * a);
  *R_N = 6.3354393272928195E+6 / rt_powd_snf(k, 3.0);
  *R_E = 6.378137E+6 / k;
}
