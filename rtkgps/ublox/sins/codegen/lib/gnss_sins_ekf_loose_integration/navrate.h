#ifndef NAVRATE_H
#define NAVRATE_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_sins_ekf_loose_integration_types.h"

extern void navrate(const double v_n[3], const double p_n[3], double nav_rate[3]);

#endif
