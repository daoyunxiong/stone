#include "rt_nonfinite.h"
#include "Gauss_projection_Positive.h"
#include "Gauss_projection_Positive_initialize.h"

void Gauss_projection_Positive_initialize(c_Gauss_projection_PositiveStac *SD)
{
  rt_InitInfAndNaN(8U);
  inited_not_empty_init(SD);
}
