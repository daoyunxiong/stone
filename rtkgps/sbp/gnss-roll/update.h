#ifndef UPDATE_H
#define UPDATE_H

#ifdef __cplusplus
//extern "C" {
#endif /* __cplusplus */


#include "Kalman.h" // Source: https://github.com/TKJElectronics/KalmanFilter
#include "rtk_gps.h"
#include <math.h>


void setup();
void loop(rtk_data_t data);
void updatePitchRoll(rtk_data_t data);
void updateYaw(rtk_data_t data);
void calibrateMag();

#ifdef __cplusplus
//}
#endif /* __cplusplus */


#endif


