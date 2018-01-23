#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "mldivide.h"
#include "xgetrf.h"

void mldivide(const double A[225], double B[225])
{
  double b_A[225];
  int ipiv[15];
  int info;
  int jBcol;
  int k;
  double temp;
  int kAcol;
  int i;
  memcpy(&b_A[0], &A[0], 225U * sizeof(double));
  xgetrf(b_A, ipiv, &info);
  for (info = 0; info < 14; info++) {
    if (ipiv[info] != info + 1) {
      jBcol = ipiv[info] - 1;
      for (k = 0; k < 15; k++) {
        temp = B[info + 15 * k];
        B[info + 15 * k] = B[jBcol + 15 * k];
        B[jBcol + 15 * k] = temp;
      }
    }
  }

  for (info = 0; info < 15; info++) {
    jBcol = 15 * info;
    for (k = 0; k < 15; k++) {
      kAcol = 15 * k;
      if (B[k + jBcol] != 0.0) {
        for (i = k + 1; i + 1 < 16; i++) {
          B[i + jBcol] -= B[k + jBcol] * b_A[i + kAcol];
        }
      }
    }
  }

  for (info = 0; info < 15; info++) {
    jBcol = 15 * info;
    for (k = 14; k >= 0; k += -1) {
      kAcol = 15 * k;
      if (B[k + jBcol] != 0.0) {
        B[k + jBcol] /= b_A[k + kAcol];
        for (i = 0; i + 1 <= k; i++) {
          B[i + jBcol] -= B[k + jBcol] * b_A[i + kAcol];
        }
      }
    }
  }
}
