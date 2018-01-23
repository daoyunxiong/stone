#ifndef GNSS_SINS_EKF_LOOSE_INTEGRATION_RTWUTIL_H
#define GNSS_SINS_EKF_LOOSE_INTEGRATION_RTWUTIL_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_sins_ekf_loose_integration_types.h"

extern double rt_atan2d_snf(double u0, double u1);
extern double rt_powd_snf(double u0, double u1);

#endif
