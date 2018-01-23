#ifndef GPP_MAIN_H
#define GPP_MAIN_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "Gauss_projection_Positive_types.h"

#ifdef __cplusplus
extern "C" 
{
#endif

int codegen_main(void);
void main_Gauss_projection_Positive(double B, double L, double Coordinate, double *x, double *y);



#ifdef __cplusplus
}
#endif


#endif
