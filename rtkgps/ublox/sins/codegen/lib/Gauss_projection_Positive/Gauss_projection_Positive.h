#ifndef GAUSS_PROJECTION_POSITIVE_H
#define GAUSS_PROJECTION_POSITIVE_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "Gauss_projection_Positive_types.h"

extern void Gauss_projection_Positive(c_Gauss_projection_PositiveStac *SD,
  double B, double L, double Coordinate, double *x, double *y);
extern void inited_not_empty_init(c_Gauss_projection_PositiveStac *SD);

#endif
