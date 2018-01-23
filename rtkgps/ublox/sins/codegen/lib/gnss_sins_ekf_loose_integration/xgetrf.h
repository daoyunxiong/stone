#ifndef XGETRF_H
#define XGETRF_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_sins_ekf_loose_integration_types.h"

extern void xgetrf(double A[225], int ipiv[15], int *info);

#endif
