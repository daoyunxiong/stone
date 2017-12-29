#ifndef GNSS_H
#define GNSS_H
 
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <libserialport.h>

#include <libsbp/sbp.h>
#include <libsbp/system.h>
#include <libsbp/navigation.h>
#include <libsbp/imu.h>
#include <libsbp/mag.h>

extern msg_gps_time_t     gps_time;
extern msg_pos_llh_t      pos_llh;
extern msg_baseline_heading_dep_a_t pos_heading;
extern msg_imu_raw_t   imu_data;
extern msg_mag_raw_t   mag_data;


void setup_port();
void heartbeat_callback(u16 sender_id, u8 len, u8 msg[], void *context);
void sbp_gps_time_callback(u16 sender_id, u8 len, u8 msg[], void *context);
void sbp_pos_llh_callback(u16 sender_id, u8 len, u8 msg[], void *context);
void sbp_heading_callback(u16 sender_id, u8 len, u8 msg[], void *context);
void sbp_imu_callback(u16 sender_id, u8 len, u8 msg[], void *context);
void sbp_mag_callback(u16 sender_id, u8 len, u8 msg[], void *context);
u32 piksi_port_read(u8 *buff, u32 n, void *context);

int open_serial_port(char* port);
int setup_sbp();
int process();

 
#ifdef __cplusplus
}
#endif /* __cplusplus */
 
#endif 
