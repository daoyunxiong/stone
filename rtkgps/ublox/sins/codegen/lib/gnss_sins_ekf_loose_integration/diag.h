#ifndef DIAG_H
#define DIAG_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_sins_ekf_loose_integration_types.h"

extern void b_diag(const double v[3], double d[9]);
extern void diag(const double v[15], double d[225]);

#endif
