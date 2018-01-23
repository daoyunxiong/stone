#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "navrate.h"
#include "expm.h"
#include "gnss_sins_ekf_loose_integration_rtwutil.h"

void navrate(const double v_n[3], const double p_n[3], double nav_rate[3])
{
  double a;
  double k;
  double R_E;
  a = 0.081819190842621486 * sin(p_n[0]);
  k = sqrt(1.0 - a * a);
  R_E = 6.378137E+6 / k;
  nav_rate[0] = v_n[1] / (R_E + p_n[2]);
  nav_rate[1] = -v_n[0] / (6.3354393272928195E+6 / rt_powd_snf(k, 3.0) + p_n[2]);
  nav_rate[2] = -v_n[1] * tan(p_n[0]) / (R_E + p_n[2]);
}
