#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "var.h"

void var(const double x[400], double y[50])
{
  int j;
  int ix;
  double xbar;
  int k;
  double r;
  double b_y;
  for (j = 0; j < 50; j++) {
    ix = j;
    xbar = x[j];
    for (k = 0; k < 7; k++) {
      ix += 50;
      xbar += x[ix];
    }

    xbar /= 8.0;
    ix = j;
    r = x[j] - xbar;
    b_y = r * r;
    for (k = 0; k < 7; k++) {
      ix += 50;
      r = x[ix] - xbar;
      b_y += r * r;
    }

    b_y /= 7.0;
    y[j] = b_y;
  }
}
