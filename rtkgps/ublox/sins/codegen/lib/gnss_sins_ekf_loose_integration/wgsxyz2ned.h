#ifndef WGSXYZ2NED_H
#define WGSXYZ2NED_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_sins_ekf_loose_integration_types.h"

extern void wgsxyz2ned(const double p_e[3], double ref_lat, double ref_lon,
  double ref_alt, double ned[3]);

#endif
