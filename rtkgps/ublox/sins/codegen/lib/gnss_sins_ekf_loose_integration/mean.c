#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "mean.h"

void mean(const double x[400], double y[50])
{
  int j;
  double s;
  int k;
  for (j = 0; j < 50; j++) {
    s = x[j];
    for (k = 0; k < 7; k++) {
      s += x[j + (k + 1) * 50];
    }

    y[j] = s;
    y[j] /= 8.0;
  }
}
