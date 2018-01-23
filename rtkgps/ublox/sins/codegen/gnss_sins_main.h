#ifndef GNSS_SINS_MAIN_H
#define GNSS_SINS_MAIN_H
#include <math.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "rt_defines.h"
#include "rt_nonfinite.h"
#include "rtwtypes.h"
#include "gnss_update_types.h"
#include "imu_update_types.h"


#ifdef __cplusplus
extern "C" 
{
#endif

void main_gnss_update(double gps_pos_llas[3],double gps_vel_neds[3]);
void main_imu_update(double tau, double imus[7]);
int gnss_sins_main(void);

#ifdef __cplusplus
}
#endif

#endif
