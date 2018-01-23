#include "rt_nonfinite.h"
#include "Gauss_projection_Positive.h"

void Gauss_projection_Positive(c_Gauss_projection_PositiveStac *SD, double B,
  double L, double Coordinate, double *x, double *y)
{
  double Num;
  double B_rad;
  double cos_B_rad;
  double t;
  double tt;
  double k;
  double N;
  double p;
  double pp;
  if (!SD->pd->inited_not_empty) {
    SD->pd->inited_not_empty = true;
    SD->pd->deg2rad = 0.017453292519943295;
    if (Coordinate == 1.0) {
      SD->pd->c = 6.399698901782711E+6;
      SD->pd->E1 = 0.006738525414683;
      SD->pd->a0 = 111134.8610828;
      SD->pd->a2 = -16036.48022;
      SD->pd->a4 = 16.82805;
      SD->pd->a6 = -0.02197;
      SD->pd->a8 = 3.0E-5;
    } else if (Coordinate == 2.0) {
      SD->pd->c = 6.3995966519880109E+6;
      SD->pd->E1 = 0.006739501819473;
      SD->pd->a0 = 111133.0046793;
      SD->pd->a2 = -16038.52818;
      SD->pd->a4 = 16.83263;
      SD->pd->a6 = -0.02198;
      SD->pd->a8 = 3.0E-5;
    } else {
      SD->pd->c = 6.3995936258E+6;
      SD->pd->E1 = 0.00673949674227;
      SD->pd->a0 = 111132.9525494;
      SD->pd->a2 = -16038.5084;
      SD->pd->a4 = 16.8326;
      SD->pd->a6 = -0.02198;
      SD->pd->a8 = 3.0E-5;
    }
  }

  Num = trunc(L / 3.0 + 0.5);
  B_rad = B * SD->pd->deg2rad;
  cos_B_rad = cos(B_rad);
  t = tan(B_rad);
  tt = t * t;
  k = SD->pd->E1 * cos_B_rad * cos_B_rad;
  N = SD->pd->c / sqrt(1.0 + k);
  p = cos_B_rad * ((L - Num * 3.0) * SD->pd->deg2rad);
  pp = p * p;
  *x = ((((SD->pd->a0 * B + SD->pd->a2 * sin(2.0 * B_rad)) + SD->pd->a4 * sin
          (4.0 * B_rad)) + SD->pd->a6 * sin(6.0 * B_rad)) + SD->pd->a8 * sin(8.0
         * B_rad)) + N * t * (1.0 + (((5.0 - tt) + (9.0 + 4.0 * k) * k) +
    (((61.0 + (tt - 58.0) * tt) + (9.0 - 11.0 * tt) * 30.0 * k) + (1385.0 +
    (-3111.0 + (543.0 - tt) * tt) * tt) * pp / 56.0) * pp / 30.0) * pp / 12.0) *
    pp / 2.0;
  *y = (Num * 1.0E+6 + 500000.0) + N * (1.0 + (((1.0 - tt) + k) + (((5.0 + tt *
    ((tt - 18.0) - 58.0 * k)) + 14.0 * k) + (61.0 + (-479.0 + (179.0 - tt) * tt)
    * tt) * pp / 42.0) * pp / 20.0) * pp / 6.0) * p;
}

void inited_not_empty_init(c_Gauss_projection_PositiveStac *SD)
{
  SD->pd->inited_not_empty = false;
}
