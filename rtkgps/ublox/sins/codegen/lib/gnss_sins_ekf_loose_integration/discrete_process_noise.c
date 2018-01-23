#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "discrete_process_noise.h"

void discrete_process_noise(const double F[225], const double G[180], double dt,
  const double Q[144], double Q_k[225])
{
  signed char I[225];
  double b_dt[180];
  int k;
  double b_I[225];
  double c_dt[225];
  int i9;
  int i10;
  memset(&I[0], 0, 225U * sizeof(signed char));
  for (k = 0; k < 15; k++) {
    I[k + 15 * k] = 1;
    for (i9 = 0; i9 < 12; i9++) {
      b_dt[k + 15 * i9] = 0.0;
      for (i10 = 0; i10 < 12; i10++) {
        b_dt[k + 15 * i9] += dt * G[k + 15 * i10] * Q[i10 + 12 * i9];
      }
    }
  }

  for (i9 = 0; i9 < 15; i9++) {
    for (i10 = 0; i10 < 15; i10++) {
      b_I[i10 + 15 * i9] = (double)I[i10 + 15 * i9] + dt * F[i10 + 15 * i9];
      c_dt[i9 + 15 * i10] = 0.0;
      for (k = 0; k < 12; k++) {
        c_dt[i9 + 15 * i10] += b_dt[i9 + 15 * k] * G[i10 + 15 * k];
      }
    }
  }

  for (i9 = 0; i9 < 15; i9++) {
    for (i10 = 0; i10 < 15; i10++) {
      Q_k[i9 + 15 * i10] = 0.0;
      for (k = 0; k < 15; k++) {
        Q_k[i9 + 15 * i10] += b_I[i9 + 15 * k] * c_dt[k + 15 * i10];
      }
    }
  }
}
