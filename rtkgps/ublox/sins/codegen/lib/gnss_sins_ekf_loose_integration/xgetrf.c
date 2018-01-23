#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "xgetrf.h"

void xgetrf(double A[225], int ipiv[15], int *info)
{
  int i16;
  int j;
  int c;
  int iy;
  int ix;
  double smax;
  int jy;
  double s;
  int b_j;
  int ijA;
  for (i16 = 0; i16 < 15; i16++) {
    ipiv[i16] = 1 + i16;
  }

  *info = 0;
  for (j = 0; j < 14; j++) {
    c = j << 4;
    iy = 0;
    ix = c;
    smax = fabs(A[c]);
    for (jy = 2; jy <= 15 - j; jy++) {
      ix++;
      s = fabs(A[ix]);
      if (s > smax) {
        iy = jy - 1;
        smax = s;
      }
    }

    if (A[c + iy] != 0.0) {
      if (iy != 0) {
        ipiv[j] = (j + iy) + 1;
        ix = j;
        iy += j;
        for (jy = 0; jy < 15; jy++) {
          smax = A[ix];
          A[ix] = A[iy];
          A[iy] = smax;
          ix += 15;
          iy += 15;
        }
      }

      i16 = (c - j) + 15;
      for (iy = c + 1; iy + 1 <= i16; iy++) {
        A[iy] /= A[c];
      }
    } else {
      *info = j + 1;
    }

    iy = c;
    jy = c + 15;
    for (b_j = 1; b_j <= 14 - j; b_j++) {
      smax = A[jy];
      if (A[jy] != 0.0) {
        ix = c + 1;
        i16 = (iy - j) + 30;
        for (ijA = 16 + iy; ijA + 1 <= i16; ijA++) {
          A[ijA] += A[ix] * -smax;
          ix++;
        }
      }

      jy += 15;
      iy += 15;
    }
  }

  if ((*info == 0) && (!(A[224] != 0.0))) {
    *info = 15;
  }
}
