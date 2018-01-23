#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "glocal.h"

void glocal(double L, double h, double g_n[3])
{
  int i;
  double a;
  double x;
  double y;
  for (i = 0; i < 3; i++) {
    g_n[i] = 0.0;
  }

  a = 0.081819190842621486 * sin(L);
  x = sin(L);
  y = h / 6.378137E+6;
  g_n[2] = ((1.0 - 2.0 * h / 6.378137E+6 * 1.0068025971715884) + 3.0 * (y * y)) *
    (9.7803253359 / sqrt(1.0 - a * a) * (1.0 + 0.001931853 * (x * x)));
}
