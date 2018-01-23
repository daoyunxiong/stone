#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "eye.h"

void eye(double I[9])
{
  int k;
  memset(&I[0], 0, 9U * sizeof(double));
  for (k = 0; k < 3; k++) {
    I[k + 3 * k] = 1.0;
  }
}
