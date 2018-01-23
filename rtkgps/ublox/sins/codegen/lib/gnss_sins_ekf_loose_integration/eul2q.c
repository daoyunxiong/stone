#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "eul2q.h"

void eul2q(const double eul[3], double q[4])
{
  double c[3];
  double s[3];
  int k;
  for (k = 0; k < 3; k++) {
    c[k] = cos(eul[k] / 2.0);
    s[k] = sin(eul[k] / 2.0);
  }

  q[0] = c[2] * c[1] * c[0] + s[2] * s[1] * s[0];
  q[1] = c[2] * c[1] * s[0] - s[2] * s[1] * c[0];
  q[2] = c[2] * s[1] * c[0] + s[2] * c[1] * s[0];
  q[3] = s[2] * c[1] * c[0] - c[2] * s[1] * s[0];
}
