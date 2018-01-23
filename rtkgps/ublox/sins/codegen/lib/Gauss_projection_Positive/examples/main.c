#include "rt_nonfinite.h"
#include "Gauss_projection_Positive.h"
#include "main.h"
#include "Gauss_projection_Positive_initialize.h"
#include "Gauss_projection_Positive_terminate.h"

static c_Gauss_projection_PositiveStac d_Gauss_projection_PositiveStac;
static c_Gauss_projection_PositivePers d_Gauss_projection_PositivePers;
static double argInit_real_T(void);
static void main_Gauss_projection_Positive(void);
static double argInit_real_T(void)
{
  return 0.0;
}

static void main_Gauss_projection_Positive(void)
{
  double x;
  double y;
  Gauss_projection_Positive(&d_Gauss_projection_PositiveStac, argInit_real_T(),
    argInit_real_T(), argInit_real_T(), &x, &y);
}

int main(int argc, const char * const argv[])
{
  (void)argc;
  (void)argv;
  d_Gauss_projection_PositiveStac.pd = &d_Gauss_projection_PositivePers;
  Gauss_projection_Positive_initialize(&d_Gauss_projection_PositiveStac);
  main_Gauss_projection_Positive();
  Gauss_projection_Positive_terminate();
  return 0;
}
