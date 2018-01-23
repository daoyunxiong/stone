#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "mrdivide.h"

void mrdivide(double A[90], const double B[36])
{
  double b_A[36];
  signed char ipiv[6];
  int k;
  int j;
  int c;
  int kBcol;
  int jp;
  int ix;
  int jAcol;
  double temp;
  int i;
  double s;
  memcpy(&b_A[0], &B[0], 36U * sizeof(double));
  for (k = 0; k < 6; k++) {
    ipiv[k] = (signed char)(1 + k);
  }

  for (j = 0; j < 5; j++) {
    c = j * 7;
    kBcol = 0;
    ix = c;
    temp = fabs(b_A[c]);
    for (k = 2; k <= 6 - j; k++) {
      ix++;
      s = fabs(b_A[ix]);
      if (s > temp) {
        kBcol = k - 1;
        temp = s;
      }
    }

    if (b_A[c + kBcol] != 0.0) {
      if (kBcol != 0) {
        ipiv[j] = (signed char)((j + kBcol) + 1);
        ix = j;
        kBcol += j;
        for (k = 0; k < 6; k++) {
          temp = b_A[ix];
          b_A[ix] = b_A[kBcol];
          b_A[kBcol] = temp;
          ix += 6;
          kBcol += 6;
        }
      }

      k = (c - j) + 6;
      for (i = c + 1; i + 1 <= k; i++) {
        b_A[i] /= b_A[c];
      }
    }

    jp = c;
    jAcol = c + 6;
    for (kBcol = 1; kBcol <= 5 - j; kBcol++) {
      temp = b_A[jAcol];
      if (b_A[jAcol] != 0.0) {
        ix = c + 1;
        k = (jp - j) + 12;
        for (i = 7 + jp; i + 1 <= k; i++) {
          b_A[i] += b_A[ix] * -temp;
          ix++;
        }
      }

      jAcol += 6;
      jp += 6;
    }
  }

  for (j = 0; j < 6; j++) {
    jp = 15 * j;
    jAcol = 6 * j;
    for (k = 1; k <= j; k++) {
      kBcol = 15 * (k - 1);
      if (b_A[(k + jAcol) - 1] != 0.0) {
        for (i = 0; i < 15; i++) {
          A[i + jp] -= b_A[(k + jAcol) - 1] * A[i + kBcol];
        }
      }
    }

    temp = 1.0 / b_A[j + jAcol];
    for (i = 0; i < 15; i++) {
      A[i + jp] *= temp;
    }
  }

  for (j = 5; j >= 0; j += -1) {
    jp = 15 * j;
    jAcol = 6 * j - 1;
    for (k = j + 2; k < 7; k++) {
      kBcol = 15 * (k - 1);
      if (b_A[k + jAcol] != 0.0) {
        for (i = 0; i < 15; i++) {
          A[i + jp] -= b_A[k + jAcol] * A[i + kBcol];
        }
      }
    }
  }

  for (kBcol = 4; kBcol >= 0; kBcol += -1) {
    if (ipiv[kBcol] != kBcol + 1) {
      jp = ipiv[kBcol] - 1;
      for (jAcol = 0; jAcol < 15; jAcol++) {
        temp = A[jAcol + 15 * kBcol];
        A[jAcol + 15 * kBcol] = A[jAcol + 15 * jp];
        A[jAcol + 15 * jp] = temp;
      }
    }
  }
}
