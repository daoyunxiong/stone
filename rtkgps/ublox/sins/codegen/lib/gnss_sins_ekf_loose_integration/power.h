#ifndef POWER_H
#define POWER_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_sins_ekf_loose_integration_types.h"

extern void b_power(const double a[3], double y[3]);
extern void power(const double a[15], double y[15]);

#endif
