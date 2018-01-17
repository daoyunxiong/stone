#include "gnss.h"
#include <math.h>
#include <time.h>
#include <pthread.h>

char *serial_port_name = NULL;
struct sp_port *piksi_port = NULL;
sbp_state_t s;
pthread_rwlock_t rwlock;  

static sbp_msg_callbacks_node_t heartbeat_callback_node;
static sbp_msg_callbacks_node_t gps_time_node;
static sbp_msg_callbacks_node_t pos_llh_node;
static sbp_msg_callbacks_node_t pos_imu_node;
static sbp_msg_callbacks_node_t pos_mag_node;
static sbp_msg_callbacks_node_t pos_heading_node;
static sbp_msg_callbacks_node_t vel_ecef_node;
static sbp_msg_callbacks_node_t vel_ned_node;

struct rtk_data_t rtk_data;

int  set_gps_data(rtk_data_t* data, u8 msg[], int type)
{
  pthread_rwlock_wrlock(&rwlock);
  switch(type){
    case SBP_MSG_GPS_TIME:
      data->gps_time = *(msg_gps_time_t *)msg;
 //     printf("gps time %d\n",(int)data->gps_time.wn);
      break;
    case SBP_MSG_POS_LLH:
      data->pos_llh = *(msg_pos_llh_t *)msg;
     // printf("thlat: %4.10lf thlon:%4.10lf \n\n ", data->pos_llh.lat, data->pos_llh.lon);
      break;
    case SBP_MSG_IMU_RAW:
      data->imu_data = *(msg_imu_raw_t* )msg;
    //  printf("imu_data x,y,z: %d,%d,%d,%d,%d,%d\n ",data->imu_data.acc_x, data->imu_data.acc_y, data->imu_data.acc_z, \
    //       data->imu_data.gyr_x, data->imu_data.gyr_y, data->imu_data.gyr_z);
      break;
    case SBP_MSG_MAG_RAW:
      data->mag_data = *(msg_mag_raw_t* )msg;
      data->heading_mag = atan2(data->mag_data.mag_y + 119, data->mag_data.mag_x + 667);
      data->heading_mag = data->heading_mag*(180.0/M_PI) + 180;
      data->heading_mag = 360 - data->heading_mag;
      if(data->heading_mag < 0)
      data->heading_mag +=360.0;
      data->heading_mag -=90.0;
      if(data->heading_mag < 0)
        data->heading_mag += 360;
      printf("magnetometer heading: %f \n ",data->heading_mag);
      break;
    case SBP_MSG_VEL_ECEF:
      data->vel_ecef_data = *(msg_vel_ecef_t* )msg;
      break;
    case SBP_MSG_VEL_NED:
      data->vel_ned_data = *(msg_vel_ned_t* )msg;
      if(abs(data->vel_ned_data.e) < 100 && abs(data->vel_ned_data.n) <100)
        data->heading_data = 0;
      else{
        data->heading_data = atan2(data->vel_ned_data.e, data->vel_ned_data.n);
        data->heading_data *= 180.0/M_PI;
      }
   //   printf("the gps heading is %f \n", data->heading_data);
      break;
    default:
      break;
  }
  pthread_rwlock_unlock(&rwlock);
}

rtk_data_t get_gps_data()
{
  pthread_rwlock_rdlock(&rwlock);
  rtk_data_t rtk_data_buf;
  rtk_data_buf = rtk_data;
  pthread_rwlock_unlock(&rwlock);
  return rtk_data_buf;
}

void setup_port()
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

void heartbeat_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  (void)sender_id, (void)len, (void)msg, (void)context;
 // fprintf(stdout, "%s\n", __FUNCTION__);
}
void sbp_gps_time_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  (void)sender_id, (void)len, (void)context;
  set_gps_data(&rtk_data, msg, SBP_MSG_GPS_TIME);
 // gps_time = *(msg_gps_time_t *)msg;
 // printf("test ok %d\n",(int)gps_time.wn);
 // fprintf(stdout, "%s\n", __FUNCTION__);
}

void sbp_pos_llh_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  (void)sender_id, (void)len, (void)context;
 //  pos_llh = *(msg_pos_llh_t *)msg;
  //pos_heading = *(msg_baseline_heading_dep_a_t *)msg;

  set_gps_data(&rtk_data, msg, SBP_MSG_POS_LLH);
 // fprintf(stdout, "%s\n", __FUNCTION__);
 // printf("lat: %4.10lf lon:%4.10lf \n ", pos_llh.lat, pos_llh.lon);
}

void sbp_heading_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
//  fprintf(stdout, "%s\n", __FUNCTION__);
  (void)sender_id, (void)len, (void)context;
 // set_gps_data(&rtk_data, msg, SBP_MSG_IMU_RAW);
//  pos_heading = *(msg_baseline_heading_dep_a_t *)msg;
//  printf("heading: %d /n", pos_heading.heading);

}

void sbp_imu_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{

//  fprintf(stdout, "%s\n", __FUNCTION__);
  (void)sender_id, (void)len, (void)context;
  set_gps_data(&rtk_data, msg, SBP_MSG_IMU_RAW);
//  imu_data = *(msg_imu_raw_t* )msg;
//  printf("imu : %d %d %d \n", imu_data.acc_x, imu_data.acc_y, imu_data.acc_z);
}

void sbp_mag_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  
 // fprintf(stdout, "%s\n", __FUNCTION__);
  (void)sender_id, (void)len, (void)context;
  set_gps_data(&rtk_data, msg, SBP_MSG_MAG_RAW);
}

void sbp_vel_ecef_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  (void)sender_id, (void)len, (void)context;
 // vel_ecef_data = *(msg_vel_ecef_t* )msg;
 // printf("ecef velx:%d vely:%d \n", vel_ecef_data.x, vel_ecef_data.y);
}


void sbp_vel_ned_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  (void)sender_id, (void)len, (void)context;  
  set_gps_data(&rtk_data, msg, SBP_MSG_VEL_NED);
  //printf("all ways ok");
}

u32 piksi_port_read(u8 *buff, u32 n, void *context)
{
  (void)context;
  u32 result;

  result = sp_blocking_read(piksi_port, buff, n, 0);
 // printf("%s %s %d\n",__FILE__, __FUNCTION__, result);
  return result;
}

int open_serial_port(char* port)
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

int setup_sbp()
{
  sbp_state_init(&s);
  sbp_register_callback(&s, SBP_MSG_HEARTBEAT, &heartbeat_callback, NULL,
                        &heartbeat_callback_node);
  sbp_register_callback(&s, SBP_MSG_GPS_TIME, &sbp_gps_time_callback,
                        NULL, &gps_time_node);
  sbp_register_callback(&s, SBP_MSG_POS_LLH, &sbp_pos_llh_callback,
                        NULL, &pos_llh_node);
  sbp_register_callback(&s, SBP_MSG_IMU_RAW, &sbp_imu_callback,
                        NULL, &pos_imu_node);
  sbp_register_callback(&s, SBP_MSG_MAG_RAW, &sbp_mag_callback,
                        NULL, &pos_mag_node);
 // sbp_register_callback(&s, SBP_MSG_BASELINE_HEADING_DEP_A, &sbp_heading_callback,
 //                     NULL, &pos_heading_node);
  sbp_register_callback(&s, SBP_MSG_VEL_ECEF, &sbp_vel_ecef_callback,
                        NULL, &vel_ecef_node);
  sbp_register_callback(&s, SBP_MSG_VEL_NED, &sbp_vel_ned_callback,
                        NULL, &vel_ned_node);
  return 0;
}

int process()
{
  while(1)
    {
       sbp_process(&s, &piksi_port_read);
    }
  return 0;
}

int close_port()
{
  int result = sp_close(piksi_port);
  if (result != SP_OK) {
    fprintf(stderr, "Cannot close %s properly!\n", serial_port_name);
  }
  sp_free_port(piksi_port);
  free(serial_port_name);
  return 0;
}

