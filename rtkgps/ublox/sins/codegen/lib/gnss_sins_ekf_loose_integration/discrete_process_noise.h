#ifndef DISCRETE_PROCESS_NOISE_H
#define DISCRETE_PROCESS_NOISE_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_sins_ekf_loose_integration_types.h"

extern void discrete_process_noise(const double F[225], const double G[180],
  double dt, const double Q[144], double Q_k[225]);

#endif
