#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "expm.h"
#include "mldivide.h"
#include "gnss_sins_ekf_loose_integration_rtwutil.h"

static void PadeApproximantOfDegree(const double A[225], unsigned char m, double
  F[225]);
static void PadeApproximantOfDegree(const double A[225], unsigned char m, double
  F[225])
{
  int i7;
  int k;
  double A2[225];
  int i8;
  double A3[225];
  double b_A[225];
  double A4[225];
  double c_A[225];
  double V[225];
  double d_A[225];
  double b_A4[225];
  double d;
  double e_A[225];
  double dv11[225];
  for (i7 = 0; i7 < 15; i7++) {
    for (k = 0; k < 15; k++) {
      A2[i7 + 15 * k] = 0.0;
      for (i8 = 0; i8 < 15; i8++) {
        A2[i7 + 15 * k] += A[i7 + 15 * i8] * A[i8 + 15 * k];
      }
    }
  }

  if (m == 3) {
    memcpy(&F[0], &A2[0], 225U * sizeof(double));
    for (k = 0; k < 15; k++) {
      F[k + 15 * k] += 60.0;
    }

    for (i7 = 0; i7 < 15; i7++) {
      for (k = 0; k < 15; k++) {
        b_A[i7 + 15 * k] = 0.0;
        for (i8 = 0; i8 < 15; i8++) {
          b_A[i7 + 15 * k] += A[i7 + 15 * i8] * F[i8 + 15 * k];
        }
      }
    }

    for (i7 = 0; i7 < 15; i7++) {
      memcpy(&F[i7 * 15], &b_A[i7 * 15], 15U * sizeof(double));
    }

    for (i7 = 0; i7 < 225; i7++) {
      V[i7] = 12.0 * A2[i7];
    }

    d = 120.0;
  } else {
    for (i7 = 0; i7 < 15; i7++) {
      for (k = 0; k < 15; k++) {
        A3[i7 + 15 * k] = 0.0;
        for (i8 = 0; i8 < 15; i8++) {
          A3[i7 + 15 * k] += A2[i7 + 15 * i8] * A2[i8 + 15 * k];
        }
      }
    }

    if (m == 5) {
      for (i7 = 0; i7 < 225; i7++) {
        F[i7] = A3[i7] + 420.0 * A2[i7];
      }

      for (k = 0; k < 15; k++) {
        F[k + 15 * k] += 15120.0;
      }

      for (i7 = 0; i7 < 15; i7++) {
        for (k = 0; k < 15; k++) {
          c_A[i7 + 15 * k] = 0.0;
          for (i8 = 0; i8 < 15; i8++) {
            c_A[i7 + 15 * k] += A[i7 + 15 * i8] * F[i8 + 15 * k];
          }
        }
      }

      for (i7 = 0; i7 < 15; i7++) {
        memcpy(&F[i7 * 15], &c_A[i7 * 15], 15U * sizeof(double));
      }

      for (i7 = 0; i7 < 225; i7++) {
        V[i7] = 30.0 * A3[i7] + 3360.0 * A2[i7];
      }

      d = 30240.0;
    } else {
      for (i7 = 0; i7 < 15; i7++) {
        for (k = 0; k < 15; k++) {
          A4[i7 + 15 * k] = 0.0;
          for (i8 = 0; i8 < 15; i8++) {
            A4[i7 + 15 * k] += A3[i7 + 15 * i8] * A2[i8 + 15 * k];
          }
        }
      }

      if (m == 7) {
        for (i7 = 0; i7 < 225; i7++) {
          F[i7] = (A4[i7] + 1512.0 * A3[i7]) + 277200.0 * A2[i7];
        }

        for (k = 0; k < 15; k++) {
          F[k + 15 * k] += 8.64864E+6;
        }

        for (i7 = 0; i7 < 15; i7++) {
          for (k = 0; k < 15; k++) {
            d_A[i7 + 15 * k] = 0.0;
            for (i8 = 0; i8 < 15; i8++) {
              d_A[i7 + 15 * k] += A[i7 + 15 * i8] * F[i8 + 15 * k];
            }
          }
        }

        for (i7 = 0; i7 < 15; i7++) {
          memcpy(&F[i7 * 15], &d_A[i7 * 15], 15U * sizeof(double));
        }

        for (i7 = 0; i7 < 225; i7++) {
          V[i7] = (56.0 * A4[i7] + 25200.0 * A3[i7]) + 1.99584E+6 * A2[i7];
        }

        d = 1.729728E+7;
      } else if (m == 9) {
        for (i7 = 0; i7 < 15; i7++) {
          for (k = 0; k < 15; k++) {
            V[i7 + 15 * k] = 0.0;
            for (i8 = 0; i8 < 15; i8++) {
              V[i7 + 15 * k] += A4[i7 + 15 * i8] * A2[i8 + 15 * k];
            }
          }
        }

        for (i7 = 0; i7 < 225; i7++) {
          F[i7] = ((V[i7] + 3960.0 * A4[i7]) + 2.16216E+6 * A3[i7]) +
            3.027024E+8 * A2[i7];
        }

        for (k = 0; k < 15; k++) {
          F[k + 15 * k] += 8.8216128E+9;
        }

        for (i7 = 0; i7 < 15; i7++) {
          for (k = 0; k < 15; k++) {
            e_A[i7 + 15 * k] = 0.0;
            for (i8 = 0; i8 < 15; i8++) {
              e_A[i7 + 15 * k] += A[i7 + 15 * i8] * F[i8 + 15 * k];
            }
          }
        }

        for (i7 = 0; i7 < 15; i7++) {
          memcpy(&F[i7 * 15], &e_A[i7 * 15], 15U * sizeof(double));
        }

        for (i7 = 0; i7 < 225; i7++) {
          V[i7] = ((90.0 * V[i7] + 110880.0 * A4[i7]) + 3.027024E+7 * A3[i7]) +
            2.0756736E+9 * A2[i7];
        }

        d = 1.76432256E+10;
      } else {
        for (i7 = 0; i7 < 225; i7++) {
          F[i7] = (3.352212864E+10 * A4[i7] + 1.05594705216E+13 * A3[i7]) +
            1.1873537964288E+15 * A2[i7];
        }

        for (k = 0; k < 15; k++) {
          F[k + 15 * k] += 3.238237626624E+16;
          for (i7 = 0; i7 < 15; i7++) {
            V[i7 + 15 * k] = (A4[i7 + 15 * k] + 16380.0 * A3[i7 + 15 * k]) +
              4.08408E+7 * A2[i7 + 15 * k];
          }
        }

        for (i7 = 0; i7 < 15; i7++) {
          for (k = 0; k < 15; k++) {
            d = 0.0;
            for (i8 = 0; i8 < 15; i8++) {
              d += A4[i7 + 15 * i8] * V[i8 + 15 * k];
            }

            b_A4[i7 + 15 * k] = d + F[i7 + 15 * k];
          }
        }

        for (i7 = 0; i7 < 15; i7++) {
          for (k = 0; k < 15; k++) {
            F[i7 + 15 * k] = 0.0;
            for (i8 = 0; i8 < 15; i8++) {
              F[i7 + 15 * k] += A[i7 + 15 * i8] * b_A4[i8 + 15 * k];
            }

            dv11[k + 15 * i7] = (182.0 * A4[k + 15 * i7] + 960960.0 * A3[k + 15 *
                                 i7]) + 1.32324192E+9 * A2[k + 15 * i7];
          }
        }

        for (i7 = 0; i7 < 15; i7++) {
          for (k = 0; k < 15; k++) {
            d = 0.0;
            for (i8 = 0; i8 < 15; i8++) {
              d += A4[i7 + 15 * i8] * dv11[i8 + 15 * k];
            }

            V[i7 + 15 * k] = ((d + 6.704425728E+11 * A4[i7 + 15 * k]) +
                              1.29060195264E+14 * A3[i7 + 15 * k]) +
              7.7717703038976E+15 * A2[i7 + 15 * k];
          }
        }

        d = 6.476475253248E+16;
      }
    }
  }

  for (k = 0; k < 15; k++) {
    V[k + 15 * k] += d;
  }

  for (k = 0; k < 225; k++) {
    V[k] -= F[k];
    F[k] *= 2.0;
  }

  mldivide(V, F);
  for (k = 0; k < 15; k++) {
    F[k + 15 * k]++;
  }
}

void expm(double A[225], double F[225])
{
  double normA;
  int j;
  boolean_T exitg2;
  double s;
  int i;
  boolean_T exitg1;
  int eint;
  static const double theta[5] = { 0.01495585217958292, 0.253939833006323,
    0.95041789961629319, 2.097847961257068, 5.3719203511481517 };

  static const unsigned char uv0[5] = { 3U, 5U, 7U, 9U, 13U };

  double b_F[225];
  int i6;
  normA = 0.0;
  j = 0;
  exitg2 = false;
  while ((!exitg2) && (j < 15)) {
    s = 0.0;
    for (i = 0; i < 15; i++) {
      s += fabs(A[i + 15 * j]);
    }

    if (rtIsNaN(s)) {
      normA = rtNaN;
      exitg2 = true;
    } else {
      if (s > normA) {
        normA = s;
      }

      j++;
    }
  }

  if (normA <= 5.3719203511481517) {
    i = 0;
    exitg1 = false;
    while ((!exitg1) && (i < 5)) {
      if (normA <= theta[i]) {
        PadeApproximantOfDegree(A, uv0[i], F);
        exitg1 = true;
      } else {
        i++;
      }
    }
  } else {
    normA /= 5.3719203511481517;
    if ((!rtIsInf(normA)) && (!rtIsNaN(normA))) {
      normA = frexp(normA, &eint);
    } else {
      eint = 0;
    }

    s = eint;
    if (normA == 0.5) {
      s = (double)eint - 1.0;
    }

    normA = rt_powd_snf(2.0, s);
    for (i = 0; i < 225; i++) {
      A[i] /= normA;
    }

    PadeApproximantOfDegree(A, 13, F);
    for (j = 0; j < (int)s; j++) {
      for (i = 0; i < 15; i++) {
        for (eint = 0; eint < 15; eint++) {
          b_F[i + 15 * eint] = 0.0;
          for (i6 = 0; i6 < 15; i6++) {
            b_F[i + 15 * eint] += F[i + 15 * i6] * F[i6 + 15 * eint];
          }
        }
      }

      for (i = 0; i < 15; i++) {
        memcpy(&F[i * 15], &b_F[i * 15], 15U * sizeof(double));
      }
    }
  }
}
