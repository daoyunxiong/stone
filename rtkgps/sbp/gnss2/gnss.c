#include "gnss.h"
/*
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
*/

char *serial_port_name = NULL;
struct sp_port *piksi_port = NULL;
sbp_state_t s;

static sbp_msg_callbacks_node_t heartbeat_callback_node;
static sbp_msg_callbacks_node_t gps_time_node;
static sbp_msg_callbacks_node_t pos_llh_node;
static sbp_msg_callbacks_node_t pos_imu_node;
static sbp_msg_callbacks_node_t pos_mag_node;
static sbp_msg_callbacks_node_t pos_heading_node;


msg_gps_time_t     gps_time;
msg_pos_llh_t      pos_llh;
msg_baseline_heading_dep_a_t pos_heading;
msg_imu_raw_t   imu_data;
msg_mag_raw_t   mag_data;


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
  //fprintf(stdout, "%s\n", __FUNCTION__);
  (void)sender_id, (void)len, (void)context;
  gps_time = *(msg_gps_time_t *)msg;
 // printf("test ok %d\n",(int)gps_time.wn);
 // fprintf(stdout, "%s\n", __FUNCTION__);
}

void sbp_pos_llh_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  (void)sender_id, (void)len, (void)context;
  pos_llh = *(msg_pos_llh_t *)msg;
  //pos_heading = *(msg_baseline_heading_dep_a_t *)msg;

 // fprintf(stdout, "%s\n", __FUNCTION__);
 // printf("lat: %4.10lf lon:%4.10lf \n ", pos_llh.lat, pos_llh.lon);
}

void sbp_heading_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  fprintf(stdout, "%s\n", __FUNCTION__);
  (void)sender_id, (void)len, (void)context;
  pos_heading = *(msg_baseline_heading_dep_a_t *)msg;
  printf("heading: %d /n", pos_heading.heading);

}

void sbp_imu_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{

//  fprintf(stdout, "%s\n", __FUNCTION__);
  (void)sender_id, (void)len, (void)context;
  imu_data = *(msg_imu_raw_t* )msg;
//  printf("imu : %d %d %d \n", imu_data.acc_x, imu_data.acc_y, imu_data.acc_z);
}

void sbp_mag_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
  
 // fprintf(stdout, "%s\n", __FUNCTION__);
  (void)sender_id, (void)len, (void)context;
  mag_data = *(msg_mag_raw_t* )msg;
 // printf("mag : %d %d %d \n", mag_data.mag_x, mag_data.mag_y, mag_data.mag_z);

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
  sbp_register_callback(&s, SBP_MSG_BASELINE_HEADING_DEP_A, &sbp_heading_callback,
                        NULL, &pos_heading_node);
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


#if 0
int main(int argc, char **argv)
{
  int opt;
  int result = 0;

  sbp_state_t s;

  if (argc <= 1) {
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  while ((opt = getopt(argc, argv, "p:")) != -1) {
    switch (opt) {
      case 'p':
        serial_port_name = (char *)calloc(strlen(optarg) + 1, sizeof(char));
        if (!serial_port_name) {
          fprintf(stderr, "Cannot allocate memory!\n");
          exit(EXIT_FAILURE);
        }
        strcpy(serial_port_name, optarg);
        break;
      case 'h':
        usage(argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (!serial_port_name) {
    fprintf(stderr, "Please supply the serial port path where the Piksi is " \
                    "connected!\n");
    exit(EXIT_FAILURE);
  }

  result = sp_get_port_by_name(serial_port_name, &piksi_port);
  if (result != SP_OK) {
    fprintf(stderr, "Cannot find provided serial port!\n");
    exit(EXIT_FAILURE);
  }

  printf("Attempting to open the serial port...\n");

  result = sp_open(piksi_port, SP_MODE_READ);
  if (result != SP_OK) {
    fprintf(stderr, "Cannot open %s for reading!\n", serial_port_name);
    exit(EXIT_FAILURE);
  }

  setup_port();

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
  sbp_register_callback(&s, SBP_MSG_BASELINE_HEADING_DEP_A, &sbp_heading_callback,
                        NULL, &pos_heading_node);

//  printf("%s %s\n",__FILE__, __FUNCTION__);

 // char rj[30];
//  char str[1000];
 // int str_i;

  while(1) {
    sbp_process(&s, &piksi_port_read);
    
 // printf("%s %s\n",__FILE__, __FUNCTION__);
 /*     str_i = 0;
      memset(str, 0, sizeof(str));

      str_i += sprintf(str + str_i, "\n\n\n\n");

      str_i += sprintf(str + str_i, "GPS Time:\n");
      str_i += sprintf(str + str_i, "\tWeek\t\t: %6d\n", (int)gps_time.wn);
      sprintf(rj, "%6.2f", ((float)gps_time.tow)/1e3);
      str_i += sprintf(str + str_i, "\tSeconds\t: %9s\n", rj);
      str_i += sprintf(str + str_i, "\n");

   printf("the time is %s\n", str);
   usleep(100);
*/
  // printf("ok \n");
   usleep(100);
  }

  result = sp_close(piksi_port);
  if (result != SP_OK) {
    fprintf(stderr, "Cannot close %s properly!\n", serial_port_name);
  }

  sp_free_port(piksi_port);

  free(serial_port_name);

  return 0;
}

#endif
