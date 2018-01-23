#ifndef QUPDT_H
#define QUPDT_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_sins_ekf_loose_integration_types.h"

extern void qupdt(const double q1[4], const double rv[3], double q[4]);

#endif
