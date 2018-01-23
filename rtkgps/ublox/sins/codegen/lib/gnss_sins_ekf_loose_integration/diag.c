#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "diag.h"

void b_diag(const double v[3], double d[9])
{
  int j;
  memset(&d[0], 0, 9U * sizeof(double));
  for (j = 0; j < 3; j++) {
    d[j + 3 * j] = v[j];
  }
}

void diag(const double v[15], double d[225])
{
  int j;
  memset(&d[0], 0, 225U * sizeof(double));
  for (j = 0; j < 15; j++) {
    d[j + 15 * j] = v[j];
  }
}
