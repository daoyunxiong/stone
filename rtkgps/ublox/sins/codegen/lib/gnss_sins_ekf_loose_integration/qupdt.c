#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "qupdt.h"

void qupdt(const double q1[4], const double rv[3], double q[4])
{
  double x;
  int i;
  double n;
  double c;
  double n_2;
  double s;
  double q2[4];
  x = 0.0;
  for (i = 0; i < 3; i++) {
    x += rv[i] * rv[i];
  }

  if (x < 1.0E-8) {
    c = 1.0 - x * (0.125 - x / 384.0);
    s = 0.5 - x * (0.020833333333333332 - x / 3840.0);
  } else {
    n = sqrt(x);
    n_2 = n / 2.0;
    c = cos(n_2);
    s = sin(n_2) / n;
  }

  q2[0] = c;
  for (i = 0; i < 3; i++) {
    q2[i + 1] = s * rv[i];
  }

  q[0] = ((q1[0] * q2[0] - q1[1] * q2[1]) - q1[2] * q2[2]) - q1[3] * q2[3];
  q[1] = ((q1[0] * q2[1] + q1[1] * q2[0]) + q1[2] * q2[3]) - q1[3] * q2[2];
  q[2] = ((q1[0] * q2[2] + q1[2] * q2[0]) + q1[3] * q2[1]) - q1[1] * q2[3];
  q[3] = ((q1[0] * q2[3] + q1[3] * q2[0]) + q1[1] * q2[2]) - q1[2] * q2[1];
  x = 0.0;
  for (i = 0; i < 4; i++) {
    x += q[i] * q[i];
  }

  if ((x > 1.000001) || (x < 0.999999)) {
    x = sqrt(x);
    for (i = 0; i < 4; i++) {
      q[i] /= x;
    }
  }
}
