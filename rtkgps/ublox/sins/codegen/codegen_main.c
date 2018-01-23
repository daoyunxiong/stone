#include "rt_nonfinite.h"
#include "Gauss_projection_Positive.h"
#include "codegen_main.h"
#include "Gauss_projection_Positive_initialize.h"
#include "Gauss_projection_Positive_terminate.h"


static c_Gauss_projection_PositiveStac d_Gauss_projection_PositiveStac;
static c_Gauss_projection_PositivePers d_Gauss_projection_PositivePers;

void main_Gauss_projection_Positive(double B, double L, double Coordinate, double *x, double *y)
{
  Gauss_projection_Positive(&d_Gauss_projection_PositiveStac, B,
    L, Coordinate, x, y);
}

//int codegen_main(int argc, const char * const argv[])
int codegen_main(void)
{
  d_Gauss_projection_PositiveStac.pd = &d_Gauss_projection_PositivePers;
  Gauss_projection_Positive_initialize(&d_Gauss_projection_PositiveStac);
  //main_Gauss_projection_Positive(B, L, Coordinate, x, y);
  //Gauss_projection_Positive_terminate();
  return 0;
}
