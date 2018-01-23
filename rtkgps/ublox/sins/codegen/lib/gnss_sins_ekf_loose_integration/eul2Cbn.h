#ifndef EUL2CBN_H
#define EUL2CBN_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_sins_ekf_loose_integration_types.h"

extern void eul2Cbn(const double PSI_nb[3], double Cbn[9]);

#endif
