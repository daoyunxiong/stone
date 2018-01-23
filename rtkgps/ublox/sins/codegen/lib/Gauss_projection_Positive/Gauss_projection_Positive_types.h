#ifndef GAUSS_PROJECTION_POSITIVE_TYPES_H
#define GAUSS_PROJECTION_POSITIVE_TYPES_H
#include "rtwtypes.h"
#ifndef typedef_c_Gauss_projection_PositivePers
#define typedef_c_Gauss_projection_PositivePers

typedef struct {
  boolean_T inited_not_empty;
  double deg2rad;
  double c;
  double E1;
  double a0;
  double a2;
  double a4;
  double a6;
  double a8;
} c_Gauss_projection_PositivePers;

#endif

#ifndef typedef_c_Gauss_projection_PositiveStac
#define typedef_c_Gauss_projection_PositiveStac

typedef struct {
  c_Gauss_projection_PositivePers *pd;
} c_Gauss_projection_PositiveStac;

#endif
#endif
