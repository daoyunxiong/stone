#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "eul2Cbn.h"

void eul2Cbn(const double PSI_nb[3], double Cbn[9])
{
  double dv7[9];
  double dv8[9];
  int i3;
  static const signed char iv0[3] = { 1, 0, 0 };

  static const signed char iv1[3] = { 0, 1, 0 };

  double dv9[9];
  double dv10[9];
  int i4;
  static const signed char iv2[3] = { 0, 0, 1 };

  int i5;
  dv7[1] = 0.0;
  dv7[4] = cos(PSI_nb[0]);
  dv7[7] = sin(PSI_nb[0]);
  dv7[2] = 0.0;
  dv7[5] = -sin(PSI_nb[0]);
  dv7[8] = cos(PSI_nb[0]);
  dv8[0] = cos(PSI_nb[1]);
  dv8[3] = 0.0;
  dv8[6] = -sin(PSI_nb[1]);
  for (i3 = 0; i3 < 3; i3++) {
    dv7[3 * i3] = iv0[i3];
    dv8[1 + 3 * i3] = iv1[i3];
  }

  dv8[2] = sin(PSI_nb[1]);
  dv8[5] = 0.0;
  dv8[8] = cos(PSI_nb[1]);
  dv10[0] = cos(PSI_nb[2]);
  dv10[3] = sin(PSI_nb[2]);
  dv10[6] = 0.0;
  dv10[1] = -sin(PSI_nb[2]);
  dv10[4] = cos(PSI_nb[2]);
  dv10[7] = 0.0;
  for (i3 = 0; i3 < 3; i3++) {
    for (i4 = 0; i4 < 3; i4++) {
      dv9[i3 + 3 * i4] = 0.0;
      for (i5 = 0; i5 < 3; i5++) {
        dv9[i3 + 3 * i4] += dv7[i3 + 3 * i5] * dv8[i5 + 3 * i4];
      }
    }

    dv10[2 + 3 * i3] = iv2[i3];
  }

  for (i3 = 0; i3 < 3; i3++) {
    for (i4 = 0; i4 < 3; i4++) {
      Cbn[i3 + 3 * i4] = 0.0;
      for (i5 = 0; i5 < 3; i5++) {
        Cbn[i3 + 3 * i4] += dv9[i4 + 3 * i5] * dv10[i5 + 3 * i3];
      }
    }
  }
}
