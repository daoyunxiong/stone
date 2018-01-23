#include "rt_nonfinite.h"
#include "gnss_sins_ekf_loose_integration.h"
#include "wgsxyz2ned.h"
#include "sind.h"
#include "cosd.h"

void wgsxyz2ned(const double p_e[3], double ref_lat, double ref_lon, double
                ref_alt, double ned[3])
{
  double d1;
  double a;
  double R_E;
  double d2;
  double p_e_ref[3];
  int i13;
  double enu[3];
  double d3;
  double d4;
  int i14;
  static const signed char b_a[9] = { 0, 1, 0, 1, 0, 0, 0, 0, -1 };

  d1 = ref_lat;
  sind(&d1);
  a = 0.081819190842621486 * d1;
  R_E = 6.378137E+6 / sqrt(1.0 - a * a);
  d1 = ref_lat;
  cosd(&d1);
  d2 = ref_lon;
  cosd(&d2);
  p_e_ref[0] = (R_E + ref_alt) * d1 * d2;
  d1 = ref_lat;
  cosd(&d1);
  d2 = ref_lon;
  sind(&d2);
  p_e_ref[1] = (R_E + ref_alt) * d1 * d2;
  d1 = ref_lat;
  sind(&d1);
  p_e_ref[2] = (0.99330562000985867 * R_E + ref_alt) * d1;
  for (i13 = 0; i13 < 3; i13++) {
    p_e_ref[i13] = p_e[i13] - p_e_ref[i13];
  }

  d1 = ref_lon;
  sind(&d1);
  d2 = ref_lon;
  cosd(&d2);
  enu[0] = -d1 * p_e_ref[0] + d2 * p_e_ref[1];
  d1 = ref_lat;
  sind(&d1);
  d2 = ref_lon;
  cosd(&d2);
  a = ref_lat;
  sind(&a);
  d3 = ref_lon;
  sind(&d3);
  d4 = ref_lat;
  cosd(&d4);
  enu[1] = (-d1 * d2 * p_e_ref[0] - a * d3 * p_e_ref[1]) + d4 * p_e_ref[2];
  d1 = ref_lat;
  cosd(&d1);
  d2 = ref_lon;
  cosd(&d2);
  a = ref_lat;
  cosd(&a);
  d3 = ref_lon;
  sind(&d3);
  d4 = ref_lat;
  sind(&d4);
  enu[2] = (d1 * d2 * p_e_ref[0] + a * d3 * p_e_ref[1]) + d4 * p_e_ref[2];
  for (i13 = 0; i13 < 3; i13++) {
    ned[i13] = 0.0;
    for (i14 = 0; i14 < 3; i14++) {
      ned[i13] += (double)b_a[i13 + 3 * i14] * enu[i14];
    }
  }
}
