/*
Filename: gnss.c
Creator:  daoyun
Date: 01-08-2018
Description:
  Get the gps data with libsbp and libserialport, written by c code.
*/

#include "rtk_gps.h"

//#define MAGNETOMETER_ORIGIN_X   -311//1039  //the original point of magnetometer output data
//#define MAGNETOMETER_ORIGIN_Y   305//491  //the original point of magnetometer output data
//#define MAGNETOMETER_ORIGIN_Z   -6//4  //the original point of magnetometer output data

//#define MAGNETOMETER_ORIGIN_X   -771  //the original point of magnetometer output data
//#define MAGNETOMETER_ORIGIN_Y   -103  //the original point of magnetometer output data
//#define MAGNETOMETER_ORIGIN_Z   136  //the original point of magnetometer output data
//
//char *serial_port_name = NULL;
//struct sp_port *piksi_port = NULL;
//sbp_state_t s;
//pthread_rwlock_t rwlock;
//
//static sbp_msg_callbacks_node_t heartbeat_callback_node;
//static sbp_msg_callbacks_node_t gps_time_node;
//static sbp_msg_callbacks_node_t pos_llh_node;
//static sbp_msg_callbacks_node_t pos_imu_node;
//static sbp_msg_callbacks_node_t pos_mag_node;
//static sbp_msg_callbacks_node_t vel_ecef_node;
//static sbp_msg_callbacks_node_t vel_ned_node;
//
//rtk_data_t rtk_data;
//
////UTMcoor utmcoor;
////WGS84Corr wgs84coor;
//
//
///*
//  function set_gps_data(rtk_data_t* data, u8 msg[], int type)
//  set gps structure data ,rtk_data_t.
//  input:
//  rtk_data_t* data, gps structure data needed to be set.
//  u8 msg[], sbp processed data
//  int type, including SBP_MSG_GPS_TIME, SBP_MSG_POS_LLH, SBP_MSG_IMU_RAW, SBP_MSG_MAG_RAW, SBP_MSG_VEL_ECEF,SBP_MSG_VEL_NED
//*/
//int  set_gps_data(rtk_data_t* data, u8 msg[], int type)
//{
//  pthread_rwlock_wrlock(&rwlock);
//  switch(type){
//    case SBP_MSG_GPS_TIME:
//      data->gps_time = *(msg_gps_time_t *)msg;
//      break;
//    case SBP_MSG_POS_LLH:
//      data->pos_llh = *(msg_pos_llh_t *)msg;
//      LatLonToUTMXY(data->pos_llh.lat, data->pos_llh.lon, 0, &data->utmcoor);
////      printf("utm x,y: %lf,%lf\n",data->utmcoor.x, data->utmcoor.y);
////      printf("gps,lat,lon %lf,%lf\n\n",data->pos_llh.lat, data->pos_llh.lon);
//      break;
//    case SBP_MSG_IMU_RAW:
//      data->imu_data = *(msg_imu_raw_t* )msg;
//      break;
//    case SBP_MSG_MAG_RAW: //output magnetometer heading angle
//      data->mag_data = *(msg_mag_raw_t* )msg;
//      data->heading_mag = atan2(data->mag_data.mag_y - MAGNETOMETER_ORIGIN_Y, data->mag_data.mag_x - MAGNETOMETER_ORIGIN_X);
//      data->heading_mag = data->heading_mag*(180.0/M_PI) + 180;
//      data->heading_mag = 360.0 - data->heading_mag;
//
//      data->heading_mag -=90.0;
//      if(data->heading_mag < 0)
//        data->heading_mag += 360;
//
////      printf("magnetometer heading: %f \n ",data->heading_mag);
//      break;
//    case SBP_MSG_VEL_ECEF:
//      data->vel_ecef_data = *(msg_vel_ecef_t* )msg;
//      break;
//    case SBP_MSG_VEL_NED: //output ned coordinate heading angle.
//      data->vel_ned_data = *(msg_vel_ned_t* )msg;
//      if(abs(data->vel_ned_data.e) < 100 && abs(data->vel_ned_data.n) <100)
//        data->heading_data = 0;
//      else{
//        data->heading_data = atan2(data->vel_ned_data.e, data->vel_ned_data.n);
//        data->heading_data *= 180.0/M_PI;
//      }
//      if(data->heading_data < 0)
//      data->heading_data +=360.0;
//      break;
//    default:
//      break;
//  }
//  pthread_rwlock_unlock(&rwlock);
//  return 0;
//}
//
///*
//  return the latest gps data rtk_data_t type
//*/
//rtk_data_t get_gps_data()
//{
//  pthread_rwlock_rdlock(&rwlock);
//  rtk_data_t rtk_data_buf;
//  rtk_data_buf = rtk_data;
//  pthread_rwlock_unlock(&rwlock);
//  return rtk_data_buf;
//}
//
///*
//  set up serial port
//*/
//void setup_port()
//{
//  int result;
//
//  printf("Attempting to configure the serial port...\n");
//
//  result = sp_set_baudrate(piksi_port, 115200);
//  if (result != SP_OK) {
//    fprintf(stderr, "Cannot set port baud rate!\n");
//    exit(EXIT_FAILURE);
//  }
//  printf("Configured the baud rate...\n");
//
//  result = sp_set_flowcontrol(piksi_port, SP_FLOWCONTROL_NONE);
//  if (result != SP_OK) {
//    fprintf(stderr, "Cannot set flow control!\n");
//    exit(EXIT_FAILURE);
//  }
//  printf("Configured the flow control...\n");
//
//  result = sp_set_bits(piksi_port, 8);
//  if (result != SP_OK) {
//    fprintf(stderr, "Cannot set data bits!\n");
//    exit(EXIT_FAILURE);
//  }
//  printf("Configured the number of data bits...\n");
//
//  result = sp_set_parity(piksi_port, SP_PARITY_NONE);
//  if (result != SP_OK) {
//    fprintf(stderr, "Cannot set parity!\n");
//    exit(EXIT_FAILURE);
//  }
//
//  printf("Configured the parity...\n");
//
//  result = sp_set_stopbits(piksi_port, 1);
//  if (result != SP_OK) {
//    fprintf(stderr, "Cannot set stop bits!\n");
//    exit(EXIT_FAILURE);
//  }
//
//  printf("Configured the number of stop bits... done.\n");
//}
//
//void heartbeat_callback(u16 sender_id, u8 len, u8 msg[], void *context)
//{
//  (void)sender_id, (void)len, (void)msg, (void)context;
//}
//
//void sbp_gps_time_callback(u16 sender_id, u8 len, u8 msg[], void *context)
//{
//  (void)sender_id, (void)len, (void)context;
//  set_gps_data(&rtk_data, msg, SBP_MSG_GPS_TIME);
// // gps_time = *(msg_gps_time_t *)msg;
// // printf("test ok %d\n",(int)gps_time.wn);
//}
//
//void sbp_pos_llh_callback(u16 sender_id, u8 len, u8 msg[], void *context)
//{
//  (void)sender_id, (void)len, (void)context;
// //  pos_llh = *(msg_pos_llh_t *)msg;
//  set_gps_data(&rtk_data, msg, SBP_MSG_POS_LLH);
// // printf("lat: %4.10lf lon:%4.10lf \n ", pos_llh.lat, pos_llh.lon);
//}
//
//void sbp_imu_callback(u16 sender_id, u8 len, u8 msg[], void *context)
//{
//  (void)sender_id, (void)len, (void)context;
////  imu_data = *(msg_imu_raw_t* )msg;
////  printf("imu : %d %d %d \n", imu_data.acc_x, imu_data.acc_y, imu_data.acc_z);
//}
//
//void sbp_mag_callback(u16 sender_id, u8 len, u8 msg[], void *context)
//{
//  (void)sender_id, (void)len, (void)context;
//  set_gps_data(&rtk_data, msg, SBP_MSG_MAG_RAW);
//}
//
//void sbp_vel_ecef_callback(u16 sender_id, u8 len, u8 msg[], void *context)
//{
//  (void)sender_id, (void)len, (void)context;
// // vel_ecef_data = *(msg_vel_ecef_t* )msg;
// // printf("ecef velx:%d vely:%d \n", vel_ecef_data.x, vel_ecef_data.y);
//}
//
//void sbp_vel_ned_callback(u16 sender_id, u8 len, u8 msg[], void *context)
//{
//  (void)sender_id, (void)len, (void)context;
//  set_gps_data(&rtk_data, msg, SBP_MSG_VEL_NED);
//}
//
//u32 piksi_port_read(u8 *buff, u32 n, void *context)
//{
//  (void)context;
//  u32 result;
//  result = sp_blocking_read(piksi_port, buff, n, 0);
//  return result;
//}
//
//int open_serial_port(char* port)
//{
//  int result = 0;
//  serial_port_name = port;
//  result = sp_get_port_by_name(serial_port_name, &piksi_port);
//  if (result != SP_OK) {
//    fprintf(stderr, "Cannot find provided serial port!\n");
//    exit(EXIT_FAILURE);
//  }
//
//  printf("Attempting to open the serial port...\n");
//
//  result = sp_open(piksi_port, SP_MODE_READ);
//  if (result != SP_OK)
//  {
//    fprintf(stderr, "Cannot open %s for reading!\n", serial_port_name);
//    exit(EXIT_FAILURE);
//  }
//  return 0;
//}
//
///*
//  set up gps data callback function
//*/
//int setup_sbp()
//{
//  sbp_state_init(&s);
//  sbp_register_callback(&s, SBP_MSG_HEARTBEAT, &heartbeat_callback, NULL,
//                        &heartbeat_callback_node);
//  sbp_register_callback(&s, SBP_MSG_GPS_TIME, &sbp_gps_time_callback,
//                        NULL, &gps_time_node);
//  sbp_register_callback(&s, SBP_MSG_POS_LLH, &sbp_pos_llh_callback,
//                        NULL, &pos_llh_node);
//  sbp_register_callback(&s, SBP_MSG_IMU_RAW, &sbp_imu_callback,
//                        NULL, &pos_imu_node);
//  sbp_register_callback(&s, SBP_MSG_MAG_RAW, &sbp_mag_callback,
//                        NULL, &pos_mag_node);
//  sbp_register_callback(&s, SBP_MSG_VEL_ECEF, &sbp_vel_ecef_callback,
//                        NULL, &vel_ecef_node);
//  sbp_register_callback(&s, SBP_MSG_VEL_NED, &sbp_vel_ned_callback,
//                        NULL, &vel_ned_node);
//  return 0;
//}
//
///*
//   thread to process gps data
//*/
//int process()
//{
//  while(1)
//    {
//       sbp_process(&s, &piksi_port_read);
//    }
//  return 0;
//}
//
///*
//  close the serial port
//*/
//int close_port()
//{
//  int result = sp_close(piksi_port);
//  if (result != SP_OK) {
//    fprintf(stderr, "Cannot close %s properly!\n", serial_port_name);
//  }
//  sp_free_port(piksi_port);
//  free(serial_port_name);
//  return 0;
//}
//int ax_i, ay_i, az_i;
//int gx_i, gy_i, gz_i;
//int mx_i, my_i, mz_i;
//float ax, ay, az;
//float gx, gy, gz;
//float mx, my, mz;
//float roll = 0;
//float pitch = 0;
//float heading = 0;
//Madgwick filter;
//MahonyAHRS filter;

void imu_data_process(msg_imu_raw_t& raw_data,imu_data_t& data){
    data.acc_x=9.8f*raw_data.acc_x/16384;
    data.acc_y=9.8f*raw_data.acc_y/16384;
    data.acc_z=9.8f*raw_data.acc_z/16384;
    data.gyr_x=(M_PI/180.0)*raw_data.gyr_x/32.8;
    data.gyr_y=(M_PI/180.0)*raw_data.gyr_y/32.8;
    data.gyr_z=(M_PI/180.0)*raw_data.gyr_z/32.8;
};



RTK_GPS::RTK_GPS(char *port):threadActive(true){
    open_serial_port(port);
    setup_port();
    setup_sbp();
    GPS_thread=new std::thread(&RTK_GPS::process,this);
}

RTK_GPS::~RTK_GPS(){
    threadActive = false;
    if(GPS_thread->joinable())
        GPS_thread->join();
    delete GPS_thread;
}

int RTK_GPS::set_gps_data(rtk_data_t* data, u8 msg[], int type)
{
    pthread_rwlock_wrlock(&rwlock);
    switch(type){
        case SBP_MSG_GPS_TIME:
            data->gps_time = *(msg_gps_time_t *)msg;
            break;
        case SBP_MSG_POS_LLH:
            data->pos_llh = *(msg_pos_llh_t *)msg;
            LatLonToUTMXY(data->pos_llh.lat, data->pos_llh.lon, 0, &data->utmcoor);
//      printf("utm x,y: %lf,%lf\n",data->utmcoor.x, data->utmcoor.y);
//      printf("tow, gps lat,lon %d, %lf,%lf\n",data->pos_llh.tow, data->pos_llh.lat, data->pos_llh.lon);
            break;
        case SBP_MSG_IMU_RAW:
            data->imu_data_raw = *(msg_imu_raw_t* )msg;
#if 0
            ax_i = rtk_data.imu_data_raw.acc_x - AX_OFFSET;
            ay_i = rtk_data.imu_data_raw.acc_y - AY_OFFSET;
            az_i = rtk_data.imu_data_raw.acc_z - AZ_OFFSET - 16384 ;//16384
            gx_i = rtk_data.imu_data_raw.gyr_x - GX_OFFSET;
            gy_i = rtk_data.imu_data_raw.gyr_y - GY_OFFSET;
            gz_i = rtk_data.imu_data_raw.gyr_z - GZ_OFFSET;
            mx_i = rtk_data.mag_data_raw.mag_x - MAGNETOMETER_ORIGIN_X;
            my_i = rtk_data.mag_data_raw.mag_y - MAGNETOMETER_ORIGIN_Y;
            mz_i = rtk_data.mag_data_raw.mag_z - MAGNETOMETER_ORIGIN_Z;
	//    if(abs(gx_i) < 10) gx_i = 0;
	//    if(abs(gy_i) < 10) gy_i = 0;
	//    if(abs(gz_i) < 10) gz_i = 0;
#ifdef DEBUG
        printf("tow acc gyo mag %d, %d,%d,%d,%d,%d,%d,%d,%d,%d \n", rtk_data.imu_data_raw.tow, ax_i, ay_i, az_i, gx_i, gy_i, gz_i, mx_i, my_i, mz_i);
#endif
            ax = RTK_GPS::convertRawAcceleration(ax_i);
            ay = RTK_GPS::convertRawAcceleration(ay_i);
            az = RTK_GPS::convertRawAcceleration(az_i);
            gx = RTK_GPS::convertRawGyro(gx_i);
            gy = RTK_GPS::convertRawGyro(gy_i);
            gz = RTK_GPS::convertRawGyro(gz_i);
            mx = - (float) mx_i;
            my = - (float) my_i;
            mz = - (float) mz_i;
                    
            heading = heading + gz * 0.02;
           filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);
           //filter.updateIMU(gx, gy, gz, ax, ay, az);
        
            // print the heading, pitch and roll
            roll = filter.getRoll();
            pitch = filter.getPitch();
           // heading = filter.getYaw();
#ifndef DEBUG        
        //  printf("Orientation: heading=%f  pitch=%f  roll=%f MAG heading=%f\n", heading, pitch, roll, rtk_data.heading_mag);
#endif
          //  printf("%f,%f,%f,%f,%f,%f,%f,%f,%f \n", ax, ay, az, gx, gy, gz, mx, my, mz);
#endif
            break;
        case SBP_MSG_MAG_RAW: //output magnetometer heading angle
            data->mag_data_raw = *(msg_mag_raw_t* )msg;
//            data->mag_data_raw.mag_y -= MAGNETOMETER_ORIGIN_Y;
//            data->mag_data_raw.mag_x -= MAGNETOMETER_ORIGIN_X;
//            data->heading_mag = atan2(data->mag_data_raw.mag_y, data->mag_data_raw.mag_x);
//            data->heading_mag = data->heading_mag*(180.0/M_PI) + 180;
//            data->heading_mag = 360.0 - data->heading_mag;
//            data->heading_mag -=90.0;
//            if(data->heading_mag < 0)
//                data->heading_mag += 360;
//
//            printf("magnetometer heading: %f,  %d,%d\n ",data->heading_mag, data->mag_data_raw.mag_x, data->mag_data_raw.mag_y);
            break;
        case SBP_MSG_VEL_ECEF:
            data->vel_ecef_data = *(msg_vel_ecef_t* )msg;
            break;
        case SBP_MSG_VEL_NED: //output ned coordinate heading angle.
            data->vel_ned_data = *(msg_vel_ned_t* )msg;
            if(abs(data->vel_ned_data.e) < 100 && abs(data->vel_ned_data.n) <100)
                data->heading_data = 0;
            else{
                data->heading_data = atan2(data->vel_ned_data.e, data->vel_ned_data.n);
                data->heading_data *= 180.0/M_PI;
            }
            if(data->heading_data < 0)
                data->heading_data +=360.0;
           // printf("gps velocity heading: %f \n", data->heading_data);
            break;
        default:
            break;
    }
    pthread_rwlock_unlock(&rwlock);
    return 0;
}

/*
  return the latest gps data rtk_data_t type
*/
rtk_data_t RTK_GPS::get_gps_data()
{
    pthread_rwlock_rdlock(&rwlock);
    rtk_data_t rtk_data_buf;
    rtk_data_buf = rtk_data;
    pthread_rwlock_unlock(&rwlock);
    return rtk_data_buf;
}

/*
  set up serial port
*/
void RTK_GPS::setup_port()
{
    int result;

    printf("Attempting to configure the serial port...\n");

    result = sp_set_baudrate(piksi_port, 115200);
    if (result != SP_OK) {
        fprintf(stderr, "Cannot set port baud rate!\n");
        exit(EXIT_FAILURE);
    }
    printf("Configured the baud rate...\n");

    result = sp_set_flowcontrol(piksi_port, SP_FLOWCONTROL_NONE);
    if (result != SP_OK) {
        fprintf(stderr, "Cannot set flow control!\n");
        exit(EXIT_FAILURE);
    }
    printf("Configured the flow control...\n");

    result = sp_set_bits(piksi_port, 8);
    if (result != SP_OK) {
        fprintf(stderr, "Cannot set data bits!\n");
        exit(EXIT_FAILURE);
    }
    printf("Configured the number of data bits...\n");

    result = sp_set_parity(piksi_port, SP_PARITY_NONE);
    if (result != SP_OK) {
        fprintf(stderr, "Cannot set parity!\n");
        exit(EXIT_FAILURE);
    }

    printf("Configured the parity...\n");

    result = sp_set_stopbits(piksi_port, 1);
    if (result != SP_OK) {
        fprintf(stderr, "Cannot set stop bits!\n");
        exit(EXIT_FAILURE);
    }

    printf("Configured the number of stop bits... done.\n");
}

void RTK_GPS::heartbeat_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
    (void)sender_id, (void)len, (void)msg, (void)context;
}

void RTK_GPS::sbp_gps_time_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
    (void)sender_id, (void)len, (void)context;
    set_gps_data(&rtk_data, msg, SBP_MSG_GPS_TIME);
    // gps_time = *(msg_gps_time_t *)msg;
    // printf("test ok %d\n",(int)gps_time.wn);
}

void RTK_GPS::sbp_pos_llh_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
    (void)sender_id, (void)len, (void)context;
    //  pos_llh = *(msg_pos_llh_t *)msg;
    set_gps_data(&rtk_data, msg, SBP_MSG_POS_LLH);
    m_GPS_sig(rtk_data);
    // printf("lat: %4.10lf lon:%4.10lf \n ", pos_llh.lat, pos_llh.lon);
}

void RTK_GPS::sbp_imu_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
    (void)sender_id, (void)len, (void)context;
    set_gps_data(&rtk_data, msg, SBP_MSG_IMU_RAW);
    m_IMU_sig(rtk_data);
//  printf("imu : %d %d %d \n", imu_data.acc_x, imu_data.acc_y, imu_data.acc_z);
}

void RTK_GPS::sbp_mag_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
    (void)sender_id, (void)len, (void)context;
    set_gps_data(&rtk_data, msg, SBP_MSG_MAG_RAW);
    m_MAG_sig(rtk_data);
}

void RTK_GPS::sbp_vel_ecef_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
    (void)sender_id, (void)len, (void)context;
    // vel_ecef_data = *(msg_vel_ecef_t* )msg;
    // printf("ecef velx:%d vely:%d \n", vel_ecef_data.x, vel_ecef_data.y);
}

void RTK_GPS::sbp_vel_ned_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
    (void)sender_id, (void)len, (void)context;
    set_gps_data(&rtk_data, msg, SBP_MSG_VEL_NED);
}

u32 RTK_GPS::piksi_port_read(u8 *buff, u32 n, void *context)
{
    (void)context;
    u32 result;
    result = sp_blocking_read(piksi_port, buff, n, 0);
    return result;
}

int RTK_GPS::open_serial_port(char* port)
{
    int result = 0;
    serial_port_name = port;
    result = sp_get_port_by_name(serial_port_name, &piksi_port);
    if (result != SP_OK) {
        fprintf(stderr, "Cannot find provided serial port!\n");
        exit(EXIT_FAILURE);
    }

    printf("Attempting to open the serial port...\n");

    result = sp_open(piksi_port, SP_MODE_READ);
    if (result != SP_OK)
    {
        fprintf(stderr, "Cannot open %s for reading!\n", serial_port_name);
        exit(EXIT_FAILURE);
    }
    return 0;
}

/*
  set up gps data callback function
*/

int RTK_GPS::setup_sbp()
{
    sbp_state_init(&s);
    sbp_register_callback(&s, SBP_MSG_HEARTBEAT, &RTK_GPS::heartbeat_callback, NULL,
                          &RTK_GPS::heartbeat_callback_node);
    sbp_register_callback(&s, SBP_MSG_GPS_TIME, &RTK_GPS::sbp_gps_time_callback,
                          NULL, &RTK_GPS::gps_time_node);
    sbp_register_callback(&s, SBP_MSG_POS_LLH, &RTK_GPS::sbp_pos_llh_callback,
                          NULL, &RTK_GPS::pos_llh_node);
    sbp_register_callback(&s, SBP_MSG_IMU_RAW, &RTK_GPS::sbp_imu_callback,
                          NULL, &RTK_GPS::pos_imu_node);
    sbp_register_callback(&s, SBP_MSG_MAG_RAW, &RTK_GPS::sbp_mag_callback,
                          NULL, &RTK_GPS::pos_mag_node);
    sbp_register_callback(&s, SBP_MSG_VEL_ECEF, &RTK_GPS::sbp_vel_ecef_callback,
                          NULL, &RTK_GPS::vel_ecef_node);
    sbp_register_callback(&s, SBP_MSG_VEL_NED, &RTK_GPS::sbp_vel_ned_callback,
                          NULL, &RTK_GPS::vel_ned_node);
    return 0;
}

/*
   thread to process gps data
*/
int RTK_GPS::process()
{
    while(threadActive)
    {
        sbp_process(&s, &piksi_port_read);
    }
    return 0;
}

/*
  close the serial port
*/
int RTK_GPS::close_port()
{
    int result = sp_close(piksi_port);
    if (result != SP_OK) {
        fprintf(stderr, "Cannot close %s properly!\n", serial_port_name);
    }
    sp_free_port(piksi_port);
    free(serial_port_name);
    return 0;
}

float RTK_GPS::convertRawAcceleration(int aRaw)
{
    // since we are using 2G range
    // -2g maps to a raw value of -32768
    // +2g maps to a raw value of 32767
    float a = (aRaw * 2.0) / 32768.0;
    return a;
}

float RTK_GPS::convertRawGyro(int gRaw)
{
    // since we are using 250 degrees/seconds range
    // -250 maps to a raw value of -32768
    // +250 maps to a raw value of 32767
    float g = (gRaw * 500.0) / 32768.0;
    return g;
}

boost::signals2::connection RTK_GPS::subscribeToGPS(const GPS_signal_t::slot_type &subscriber){
    return m_GPS_sig.connect(subscriber);
}
boost::signals2::connection RTK_GPS::subscribeToIMU(const IMU_signal_t::slot_type &subscriber){
    return m_IMU_sig.connect(subscriber);
}
boost::signals2::connection RTK_GPS::subscribeToMAG(const MAG_signal_t::slot_type &subscriber){
    return m_MAG_sig.connect(subscriber);
}

sbp_msg_callbacks_node_t RTK_GPS::heartbeat_callback_node;
sbp_msg_callbacks_node_t RTK_GPS::gps_time_node;
sbp_msg_callbacks_node_t RTK_GPS::pos_llh_node;
sbp_msg_callbacks_node_t RTK_GPS::pos_imu_node;
sbp_msg_callbacks_node_t RTK_GPS::pos_mag_node;
sbp_msg_callbacks_node_t RTK_GPS::vel_ecef_node;
sbp_msg_callbacks_node_t RTK_GPS::vel_ned_node;

rtk_data_t RTK_GPS::rtk_data;

struct sp_port *RTK_GPS::piksi_port=NULL ;
pthread_rwlock_t RTK_GPS::rwlock;

GPS_signal_t RTK_GPS::m_GPS_sig;
IMU_signal_t RTK_GPS::m_IMU_sig;
MAG_signal_t RTK_GPS::m_MAG_sig;

