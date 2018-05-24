/*
Filename: gnss.h
Creator:  daoyun
Date: 01-08-2018
Description:
  This module used to process the gps data with sbp library.
  To get the gps, imu, magnetic data;
*/

#ifndef GNSS_H_
#define GNSS_H_

//#ifdef __cplusplus
//extern "C" {
//#endif /* __cplusplus */

#include <cstdio>
#include <cstdlib>
#include <string>
#include <thread>
#include <unistd.h>
#include <ctime>
#include <libserialport.h>

#include <libsbp/sbp.h>
#include <libsbp/system.h>
#include <libsbp/navigation.h>
#include <libsbp/imu.h>
#include <libsbp/mag.h>

#include "utm.h"
//#include "MadgwickAHRS.h"
//#include "MahonyAHRS.h"
#include "Kalman.h"

#include <boost/signals2/signal.hpp>

/*
rtk_data_t : the gps data structure
gps_time :
pos_llh:  lat lon position
imu_data: imu
mag_data: magnetic data
vel_ecef_data : ecef coordinate velocity
vel_ned_data:  ned coordinate velocity
heading_data: ned coordinate gps velocity heading
heading_mag:  heading angle of magnetometer
*/
//
typedef struct {
    msg_gps_time_t gps_time;
    msg_pos_llh_t pos_llh;
    msg_imu_raw_t imu_data_raw;
    msg_mag_raw_t mag_data_raw;
    msg_vel_ecef_t vel_ecef_data;
    msg_vel_ned_t vel_ned_data;
    msg_baseline_ned_t baseline_ned_data;
    double heading_data;
    double heading_mag;
    float heading_kalman;
    UTMCoor utmcoor;
    WGS84Corr wgs84coor;
}rtk_data_t;

//void setup_port();
//void heartbeat_callback(u16 sender_id, u8 len, u8 msg[], void *context);
//void sbp_gps_time_callback(u16 sender_id, u8 len, u8 msg[], void *context);
//void sbp_pos_llh_callback(u16 sender_id, u8 len, u8 msg[], void *context);
//void sbp_imu_callback(u16 sender_id, u8 len, u8 msg[], void *context);
//void sbp_mag_callback(u16 sender_id, u8 len, u8 msg[], void *context);
//void sbp_vel_ned_callback(u16 sender_id, u8 len, u8 msg[], void *context);
//
//u32 piksi_port_read(u8 *buff, u32 n, void *context);
//int open_serial_port(char* port);
//int setup_sbp();
//int process();
//int close_port();

 
//#ifdef __cplusplus
//}
//#endif /* __cplusplus */
typedef boost::signals2::signal<void(rtk_data_t &)> GPS_signal_t;
typedef boost::signals2::signal<void(rtk_data_t &)> IMU_signal_t;
typedef boost::signals2::signal<void(rtk_data_t &)> MAG_signal_t;
typedef boost::signals2::signal<void(rtk_data_t &)> NED_signal_t;
typedef boost::signals2::signal<void(rtk_data_t &)> BaselineNED_signal_t;

#define DEBUG 0
//#define ADJUST 0
//#define BASESTATION 0
#define ROVER 0

#ifdef ADJUST
#define MAGNETOMETER_ORIGIN_X   0  //the original point of magnetometer output data
#define MAGNETOMETER_ORIGIN_Y   0  //the original point of magnetometer output data
#define MAGNETOMETER_ORIGIN_Z   0  //the original point of magnetometer output data
#define AX_OFFSET 0
#define AY_OFFSET 0
#define AZ_OFFSET 0
#define GX_OFFSET 0
#define GY_OFFSET 0
#define GZ_OFFSET 0
#endif

#ifdef BASESTATION
#define AX_OFFSET 700
#define AY_OFFSET 579
#define AZ_OFFSET -17455
#define GX_OFFSET 100
#define GY_OFFSET 22
#define GZ_OFFSET 110

#define MAGNETOMETER_ORIGIN_X   -275//1039  //the original point of magnetometer output data
#define MAGNETOMETER_ORIGIN_Y   333//491  //the original point of magnetometer output data
#define MAGNETOMETER_ORIGIN_Z   -4//4  //the original point of magnetometer output data
#endif 

#ifdef ROVER
#define AX_OFFSET -1003
#define AY_OFFSET 73
#define AZ_OFFSET -16902
#define GX_OFFSET 11
#define GY_OFFSET -19
#define GZ_OFFSET 15

#define MAGNETOMETER_ORIGIN_X   -371  //the original point of magnetometer output data
#define MAGNETOMETER_ORIGIN_Y   242  //the original point of magnetometer output data
#define MAGNETOMETER_ORIGIN_Z   165  //the original point of magnetometer output data
#endif

typedef struct{
    double acc_x;    /**< Acceleration in the IMU frame X axis */
    double acc_y;    /**< Acceleration in the IMU frame Y axis */
    double acc_z;    /**< Acceleration in the IMU frame Z axis */
    double gyr_x;    /**< Angular rate around IMU frame X axis */
    double gyr_y;    /**< Angular rate around IMU frame Y axis */
    double gyr_z;    /**< Angular rate around IMU frame Z axis */
}imu_data_t;

void imu_data_process(msg_imu_raw_t&,imu_data_t&);

class RTK_GPS{
public:
    RTK_GPS(char *port);
    ~RTK_GPS();
    rtk_data_t get_gps_data();

    boost::signals2::connection subscribeToGPS(const GPS_signal_t::slot_type &subscriber);
    boost::signals2::connection subscribeToIMU(const IMU_signal_t::slot_type &subscriber);
    boost::signals2::connection subscribeToMAG(const MAG_signal_t::slot_type &subscriber);
    boost::signals2::connection subscribeToNED(const NED_signal_t::slot_type &subscriber);
    boost::signals2::connection subscribeToBaselineNED(const NED_signal_t::slot_type &subscriber);

    void setup_port();
    int open_serial_port(char* port);
    int setup_sbp();
    int process();
    int close_port();

private:
    int static set_gps_data(rtk_data_t &data, u8 msg[], int type);
    u32 static piksi_port_read(u8 *buff, u32 n, void *context);

    void static heartbeat_callback(u16 sender_id, u8 len, u8 msg[], void *context);
    void static sbp_gps_time_callback(u16 sender_id, u8 len, u8 msg[], void *context);
    void static sbp_pos_llh_callback(u16 sender_id, u8 len, u8 msg[], void *context);
    void static sbp_imu_callback(u16 sender_id, u8 len, u8 msg[], void *context);
    void static sbp_mag_callback(u16 sender_id, u8 len, u8 msg[], void *context);
    void static sbp_vel_ecef_callback(u16 sender_id, u8 len, u8 msg[], void *context);
    void static sbp_vel_ned_callback(u16 sender_id, u8 len, u8 msg[], void *context);
    void static sbp_baseline_ned_callback(u16 sender_id, u8 len, u8 msg[], void *context);

    static sbp_msg_callbacks_node_t heartbeat_callback_node;
    static sbp_msg_callbacks_node_t gps_time_node;
    static sbp_msg_callbacks_node_t pos_llh_node;
    static sbp_msg_callbacks_node_t pos_imu_node;
    static sbp_msg_callbacks_node_t pos_mag_node;
    static sbp_msg_callbacks_node_t vel_ecef_node;
    static sbp_msg_callbacks_node_t vel_ned_node;
    static sbp_msg_callbacks_node_t baseline_ned_node;

    static rtk_data_t rtk_data;

    char *serial_port_name = NULL;
    static struct sp_port *piksi_port ;
    sbp_state_t s;
    static pthread_rwlock_t rwlock;
    bool threadActive;
    std::thread *GPS_thread;

    double vw;

 
//subscribe message
    static GPS_signal_t m_GPS_sig;
    static IMU_signal_t m_IMU_sig;
    static MAG_signal_t m_MAG_sig;
    static NED_signal_t m_NED_sig;
    static BaselineNED_signal_t m_BaselineNED_sig;

//kalman filter
//    static int ax_i, ay_i, az_i;
//    static int gx_i, gy_i, gz_i;
//    static int mx_i, my_i, mz_i;
//    static float ax, ay, az;
//    static float gx, gy, gz;
//    static float mx, my, mz;
//    static float roll;
//    static float pitch;
//    static float heading;
    
//    static Madgwick *filter;
//    static Kalman *kalman_filter;
    //MahonyAHRS filter;

//    static float newangle;
//    static float dt;
//    static float angle;

    //static int buf[8][3];
public:
    static float convertRawGyro(int gRaw);
    static float convertRawAcceleration(int aRaw);
    void static kalman_heading_update(rtk_data_t &data);
    void static magnetic_heading_update(rtk_data_t &data);
  //  static Madgwick filter;
  //  static int ax_i, ay_i, az_i;
  //  static int gx_i, gy_i, gz_i;
  //  static int mx_i, my_i, mz_i;
  //  static float ax, ay, az;
  //  static float gx, gy, gz;
  //  static float mx, my, mz;
  //  static float roll, pitch, heading;
};

#endif //GNSS_H_
