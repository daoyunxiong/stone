/*
Filename: gnss.c
Creator:  daoyun
Date: 01-08-2018
Description:
  Get the gps data with libsbp and libserialport, written by c code.
*/

#include "rtk_gps.h"

#define RAD_TO_DEG (180 / M_PI)
#define DEG_TO_RAD (M_PI / 180)
#define GRAVITY 9.80665

float RTK_GPS::yaw = 0;
float RTK_GPS::roll = 0;
float RTK_GPS::pitch = 0;
float RTK_GPS::xcoor = 0;
float RTK_GPS::ycoor = 0;
float RTK_GPS::vn_g = 0;
float RTK_GPS::ve_g = 0;

int buf[8][3];
int buf_acc[8][3];
//Madgwick filter;
//MahonyAHRS filter;
Kalman kalman_filter;
//Kalman kalmanX, kalmanY, kalmanZ;

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

    memset(buf, 0, sizeof(buf));
}

RTK_GPS::~RTK_GPS(){
    threadActive = false;
    if(GPS_thread->joinable())
        GPS_thread->join();
    delete GPS_thread;
}

int RTK_GPS::set_gps_data(rtk_data_t &data, u8 msg[], int type)
{
    pthread_rwlock_wrlock(&rwlock);
    switch(type){
        case SBP_MSG_GPS_TIME:
            data.gps_time = *(msg_gps_time_t *)msg;
            break;
        case SBP_MSG_POS_LLH:
            data.pos_llh = *(msg_pos_llh_t *)msg;
            LatLonToUTMXY(data.pos_llh.lat, data.pos_llh.lon, 0, &data.utmcoor);
            if(data.pos_llh.flags != 0){
              xcoor = data.utmcoor.x;
              ycoor = data.utmcoor.y;
            }
//      printf("utm x,y: %lf,%lf\n",data->utmcoor.x, data->utmcoor.y);
//      printf("tow, gps lat,lon %d, %lf,%lf\n",data->pos_llh.tow, data->pos_llh.lat, data->pos_llh.lon);
            break;
        case SBP_MSG_IMU_RAW:
            data.imu_data_raw = *(msg_imu_raw_t* )msg;
            kalman_heading_update(data);
#if 0
     int ax_i = rtk_data.imu_data_raw.acc_x - AX_OFFSET;
     int ay_i = rtk_data.imu_data_raw.acc_y - AY_OFFSET;
     int az_i = rtk_data.imu_data_raw.acc_z - AZ_OFFSET - 16384 ;//16384
     int gx_i = rtk_data.imu_data_raw.gyr_x - GX_OFFSET;
     int gy_i = rtk_data.imu_data_raw.gyr_y - GY_OFFSET;
     int gz_i = rtk_data.imu_data_raw.gyr_z - GZ_OFFSET;
     int mx_i = rtk_data.mag_data_raw.mag_x - MAGNETOMETER_ORIGIN_X;
     int my_i = rtk_data.mag_data_raw.mag_y - MAGNETOMETER_ORIGIN_Y;
     int mz_i = rtk_data.mag_data_raw.mag_z - MAGNETOMETER_ORIGIN_Z;
	//    if(abs(gx_i) < 10) gx_i = 0;
	//    if(abs(gy_i) < 10) gy_i = 0;
	//    if(abs(gz_i) < 10) gz_i = 0;
#ifdef DEBUG
        printf("tow acc gyo mag %d, %d,%d,%d,%d,%d,%d,%d,%d,%d \n", rtk_data.imu_data_raw.tow, ax_i, ay_i, az_i, gx_i, gy_i, gz_i, mx_i, my_i, mz_i);
#endif
     float ax = RTK_GPS::convertRawAcceleration(ax_i);
     float ay = RTK_GPS::convertRawAcceleration(ay_i);
     float az = RTK_GPS::convertRawAcceleration(az_i);
     float gx = RTK_GPS::convertRawGyro(gx_i);
     float gy = RTK_GPS::convertRawGyro(gy_i);
     float gz = RTK_GPS::convertRawGyro(gz_i);
     float mx = - (float) mx_i;
     float my = - (float) my_i;
     float mz = - (float) mz_i;
                    
     float heading = heading + gz * 0.02;
          // filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);
           //filter.updateIMU(gx, gy, gz, ax, ay, az);
        
            // print the heading, pitch and roll
           // roll = filter.getRoll();
           // pitch = filter.getPitch();
           // heading = filter.getYaw();
#ifndef DEBUG        
        //  printf("Orientation: heading=%f  pitch=%f  roll=%f MAG heading=%f\n", heading, pitch, roll, rtk_data.heading_mag);
#endif
          //  printf("%f,%f,%f,%f,%f,%f,%f,%f,%f \n", ax, ay, az, gx, gy, gz, mx, my, mz);
#endif
            break;
        case SBP_MSG_MAG_RAW: //output magnetometer heading angle
            data.mag_data_raw = *(msg_mag_raw_t* )msg;
            magnetic_heading_update(data);
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
            data.vel_ecef_data = *(msg_vel_ecef_t* )msg;
            break;
        case SBP_MSG_VEL_NED: //output ned coordinate heading angle.
            data.vel_ned_data = *(msg_vel_ned_t* )msg;
            if(abs(data.vel_ned_data.e) < 100 && abs(data.vel_ned_data.n) <100)
                data.heading_data = 0;
            else{
                data.heading_data = atan2(data.vel_ned_data.e, data.vel_ned_data.n);
                data.heading_data *= 180.0/M_PI;
            }
            if(data.heading_data < 0)
                data.heading_data += 360.0;
           // printf("gps velocity heading: %f \n", data->heading_data);

            if(data.vel_ned_data.flags != 0){
              vn_g = (float)(data.vel_ned_data.n * 0.01 ); 
              ve_g = (float)(data.vel_ned_data.e * 0.01 );
            }
            break;
        case SBP_MSG_BASELINE_NED:
            data.baseline_ned_data = *(msg_baseline_ned_t* )msg;
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

void RTK_GPS::kalman_heading_update(rtk_data_t &data)
{
    //imu_data_process(rtk_data.imu_data_raw,temp);
 /*   std::cout  << rtk_data.imu_data_raw.tow << ","
             << temp.imu_data_raw.acc_x << ","
             << temp.imu_data_raw.acc_y << ","
             << temp.imu_data_raw.acc_z << ","
             << temp.imu_data_raw.gyr_x << ","
             << temp.imu_data_raw.gyr_y << ","
             << temp.imu_data_raw.gyr_z << "\n";
*/
    int ax_i, ay_i, az_i;
    int gx_i, gy_i, gz_i;
    int mx_i, my_i, mz_i;
    float ax, ay, az;
    float gx, gy, gz;
    float mx, my, mz;
    float heading;
  
    float newangle;
    static float dt = 0.02f;
    static float angle = 0.0f;
    static float angle_gyr = 0.0f;
/*
    static double kalAngleX;
    static double kalAngleY;
    static double kalAngleZ;
    static double compAngleX, compAngleY, compAngleZ;
    static int acc_cnt;
  */
    static int acc_cnt = 0;

    ax_i = data.imu_data_raw.acc_x - AX_OFFSET;
    ay_i = data.imu_data_raw.acc_y - AY_OFFSET;
    az_i = data.imu_data_raw.acc_z - AZ_OFFSET - 16384 ;//16384
    gx_i = data.imu_data_raw.gyr_x - GX_OFFSET;
    gy_i = data.imu_data_raw.gyr_y - GY_OFFSET;
    gz_i = data.imu_data_raw.gyr_z - GZ_OFFSET;
    mx_i = data.mag_data_raw.mag_x - MAGNETOMETER_ORIGIN_X;
    my_i = data.mag_data_raw.mag_y - MAGNETOMETER_ORIGIN_Y;
    mz_i = data.mag_data_raw.mag_z - MAGNETOMETER_ORIGIN_Z;


    buf_acc[acc_cnt][0] = ax_i;
    buf_acc[acc_cnt][1] = ay_i;
    buf_acc[acc_cnt][2] = az_i;

    acc_cnt ++;
    if(acc_cnt > 7)
      acc_cnt = 0;
  
    int sumx_acc = 0;
    int sumy_acc = 0;
    int sumz_acc = 0;
    for(int j = 0; j < 8; j++){
      sumx_acc += buf_acc[j][0];
      sumy_acc += buf_acc[j][1];
      sumz_acc += buf_acc[j][2];
    }
  
    ax_i = sumx_acc / 8;
    ay_i = sumy_acc / 8;
    az_i = sumz_acc / 8;

//    if(abs(gx_i) < 10) gx_i = 0;
//    if(abs(gy_i) < 10) gy_i = 0;
//    if(abs(gz_i) < 10) gz_i = 0;
    ax = RTK_GPS::convertRawAcceleration(ax_i) * GRAVITY;
    ay = RTK_GPS::convertRawAcceleration(ay_i) * GRAVITY;
    az = RTK_GPS::convertRawAcceleration(az_i) * GRAVITY;
    gx = RTK_GPS::convertRawGyro(gx_i);
    gy = RTK_GPS::convertRawGyro(gy_i);
    gz = RTK_GPS::convertRawGyro(gz_i);
    mx = - (float) mx_i;
    my = - (float) my_i;
    mz = - (float) mz_i;

//    static double sumx, sumy, sumz;
//    static double vx, vy, vz;

/*
    vx += ax * dt;
    vy += ay * dt;
    vz += vz * dt;
    sumx += vx * dt + 0.5 * ax * dt * dt; 
    sumy += vy * dt + 0.5 * ay * dt * dt; 
    sumz += vz * dt + 0.5 * az * dt * dt; 
*/    
     
 //   printf("%f, %f, %f %f %f %f %f %f  x y z \n", ax, ay, az, vx, vy, vz, sumx, sumy);
//update roll pitch
    roll = atan2(ay_i, az_i) * 180 / M_PI;
    pitch = atan(-ax_i / sqrt(ay_i * ay_i + az_i * az_i)) * 180 / M_PI;

    float gyroXrate = gx; // Convert to deg/s
    float gyroYrate = gy; // Convert to deg/s
    float gyroZrate = gz; // Convert to deg/s

#if 0
    if ((roll < -90 && kalAngleX > 90) || (roll > 90 && kalAngleX < -90)) {
      kalmanX.setAngle(roll);
     // compAngleX = roll;
     // kalAngleX = roll;
     // gyroXangle = roll;
    } else
      kalAngleX = kalmanX.getAngle(roll, gyroXrate, dt); // Calculate the angle using a Kalman filter

    if (abs(kalAngleX) > 90)
      gyroYrate = -gyroYrate; // Invert rate, so it fits the restricted accelerometer reading
    kalAngleY = kalmanY.getAngle(pitch, gyroYrate, dt);

// update yaw angle
  double rollAngle = kalAngleX * DEG_TO_RAD;
  double pitchAngle = kalAngleY * DEG_TO_RAD;
  double Bfy = mz_i * sin(rollAngle) - mz_i * cos(rollAngle);
  double Bfx = mx_i * cos(pitchAngle) + my_i * sin(pitchAngle) * sin(rollAngle) + mz_i * sin(pitchAngle) * cos(rollAngle);
 
  yaw = atan2(-Bfy, Bfx) * RAD_TO_DEG;
  yaw *= -1;

  if ((yaw < -90 && kalAngleZ > 90) || (yaw > 90 && kalAngleZ < -90)) {
    kalmanZ.setAngle(yaw);
  } else
    kalAngleZ = kalmanZ.getAngle(yaw, gyroZrate, dt);

  compAngleX = 0.93 * (compAngleX + gyroXrate * dt) + 0.07 * roll; // Calculate the angle using a Complimentary filter
  compAngleY = 0.93 * (compAngleY + gyroYrate * dt) + 0.07 * pitch;
  compAngleZ = 0.93 * (compAngleZ + gyroZrate * dt) + 0.07 * yaw;

#endif
//  printf("%f,%f,%f roll pith yaw\n", roll, pitch, yaw);
 // printf("%f,%f,%f croll cpith cyaw\n", compAngleX, compAngleY, compAngleZ);
// filter.update(gx, gy, gz, ax, ay, az, mx, my, mz);
//filter.updateIMU(gx, gy, gz, ax, ay, az);
// roll = filter.getRoll();
// pitch = filter.getPitch();
//float  heading_ahrs = filter.getYaw();

    newangle = data.heading_mag;
   // if((newangle < -90 && angle > 90) || (newangle > 90 && angle < -90))
   //printf("mag_angle, fution angle %f,%f\n", newangle, angle
    if((newangle < 90 && angle > 270) || (newangle > 270 && angle < 90))
    {
      kalman_filter.setAngle(newangle);
      angle = newangle;
    }
    else
    {
      angle = kalman_filter.getAngle(newangle, gz, dt);
    }
    
//  if(angle < 0)
//    angle = angle + 360.0;
//  angle += 90;
//  if(angle >360)
//    angle = angle - 360;
  
    if(mz_i < 20 && mz_i > -20){
      angle_gyr = angle;
      data.heading_kalman = angle_gyr;
    }
    else{
      data.heading_kalman = angle_gyr + gz * dt;
      angle_gyr += gz * dt;
    }
       
//    printf("%f, %f %f %f\n\n", angle , rtk_data.heading_kalman, roll, pitch);
#ifndef DEBUG        
   // printf("Orientation: heading=%f  pitch=%f  roll=%f MAG heading=%f kalman=%f gpsheading=%f\n", heading, pitch, roll, rtk_data.heading_mag, angle, rtk_data.heading_data);
#endif

#ifdef DEBUG
   // printf("tow acc gyo mag %d,%d,%d,%d,%d,%d,%d,%d,%d,%d \n", data.imu_data_raw.tow, ax_i, ay_i, az_i, gx_i, gy_i, gz_i, mx_i, my_i, mz_i);
#endif

  float accn = ax * cos((90 - angle) * M_PI/180) + ay * sin((90 - angle) * M_PI/180);
  float acce = ay * cos((90 - angle) * M_PI/180) - ax * sin((90 - angle) * M_PI/180);

//  float accn = ax * cos(M_PI/3) + ay * sin(M_PI/3);
//  float acce = ay * cos(M_PI/3) - ax * sin(M_PI/180);

  ycoor += vn_g * dt + 0.5 * accn * dt * dt;
  xcoor += ve_g * dt + 0.5 * acce * dt * dt;

  vn_g += accn * dt;
  ve_g += acce * dt;

  data.utmcoor.x = xcoor;
  data.utmcoor.y = ycoor;

 // printf("%f %f %f %f %f %f %f %f %f %f angle accn acce ax ay az x y vn_g ve_g\n",angle, accn, acce, ax, ay, az, xcoor, ycoor, vn_g, ve_g);
//  printf("%f %f x,y \n", xcoor, ycoor);
}

void RTK_GPS:: magnetic_heading_update(rtk_data_t &data)
{
 static int count = 0;

/*
  std::cout  << rtk_data.mag_data_raw.tow << ","
  << rtk_data.mag_data_raw.mag_x << ","
  << rtk_data.mag_data_raw.mag_y << ","
  << rtk_data.mag_data_raw.mag_z << "  ";
*/
  float mx_i = data.mag_data_raw.mag_x - MAGNETOMETER_ORIGIN_X;
  float my_i = (data.mag_data_raw.mag_y - MAGNETOMETER_ORIGIN_Y) *0.900862069;
  float mz_i = data.mag_data_raw.mag_z - MAGNETOMETER_ORIGIN_Z;

  buf[count][0] = mx_i;
  buf[count][1] = my_i;
  buf[count][2] = mz_i;

  float sumx = 0;
  float sumy = 0;
  for(int j = 0; j<8; j++)
  {
    sumx += buf[j][0];
    sumy += buf[j][1];
  }

  mx_i = sumx / 8;
  my_i = sumy / 8;

 // data.heading_mag = atan2(-my_i, mx_i) * 180.0 / M_PI;

  data.heading_mag = atan2(my_i, mx_i);
  data.heading_mag = data.heading_mag*(180.0/M_PI) + 180;
  data.heading_mag = 360.0 - data.heading_mag;
  data.heading_mag -= 95.0;
  if(data.heading_mag < 0)
    data.heading_mag += 360;

 // data.heading_mag = data.heading_mag + 85;
 // if(data.heading_mag < 0)
 //   data.heading_mag += 360.0;
 //  data.heading_mag += 180.0;
 // if(data.heading_mag > 360)
 //   data.heading_mag = data.heading_mag - 360;
 //  printf("heading mag %f \n", data.heading_mag);

  count ++;
  if(count > 7)
    count = 0;
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
    set_gps_data(rtk_data, msg, SBP_MSG_GPS_TIME);
    // gps_time = *(msg_gps_time_t *)msg;
    // printf("test ok %d\n",(int)gps_time.wn);
}

void RTK_GPS::sbp_pos_llh_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
    (void)sender_id, (void)len, (void)context;
    //  pos_llh = *(msg_pos_llh_t *)msg;
    set_gps_data(rtk_data, msg, SBP_MSG_POS_LLH);
    m_GPS_sig(rtk_data);
    // printf("lat: %4.10lf lon:%4.10lf \n ", pos_llh.lat, pos_llh.lon);
}

void RTK_GPS::sbp_imu_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
    (void)sender_id, (void)len, (void)context;
    set_gps_data(rtk_data, msg, SBP_MSG_IMU_RAW);
    m_IMU_sig(rtk_data);
//  printf("imu : %d %d %d \n", imu_data.acc_x, imu_data.acc_y, imu_data.acc_z);
}

void RTK_GPS::sbp_mag_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
    (void)sender_id, (void)len, (void)context;
    set_gps_data(rtk_data, msg, SBP_MSG_MAG_RAW);
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
    set_gps_data(rtk_data, msg, SBP_MSG_VEL_NED);
    m_NED_sig(rtk_data);
}

void RTK_GPS::sbp_baseline_ned_callback(u16 sender_id, u8 len, u8 msg[], void *context)
{
    (void)sender_id, (void)len, (void)context;
    set_gps_data(rtk_data, msg, SBP_MSG_BASELINE_NED);
    m_BaselineNED_sig(rtk_data);
 //   printf("ned tow:%d, n:%d, e:%d\n", rtk_data.baseline_ned_data.tow, rtk_data.baseline_ned_data.n, rtk_data.baseline_ned_data.e);
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
    sbp_register_callback(&s, SBP_MSG_BASELINE_NED, &RTK_GPS::sbp_baseline_ned_callback,
                          NULL, &RTK_GPS::baseline_ned_node);
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
boost::signals2::connection RTK_GPS::subscribeToNED(const NED_signal_t::slot_type &subscriber){
    return m_NED_sig.connect(subscriber);
}
boost::signals2::connection RTK_GPS::subscribeToBaselineNED(const BaselineNED_signal_t::slot_type &subscriber){
    return m_BaselineNED_sig.connect(subscriber);
}

sbp_msg_callbacks_node_t RTK_GPS::heartbeat_callback_node;
sbp_msg_callbacks_node_t RTK_GPS::gps_time_node;
sbp_msg_callbacks_node_t RTK_GPS::pos_llh_node;
sbp_msg_callbacks_node_t RTK_GPS::pos_imu_node;
sbp_msg_callbacks_node_t RTK_GPS::pos_mag_node;
sbp_msg_callbacks_node_t RTK_GPS::vel_ecef_node;
sbp_msg_callbacks_node_t RTK_GPS::vel_ned_node;
sbp_msg_callbacks_node_t RTK_GPS::baseline_ned_node;

rtk_data_t RTK_GPS::rtk_data;

struct sp_port *RTK_GPS::piksi_port=NULL ;
pthread_rwlock_t RTK_GPS::rwlock;

GPS_signal_t RTK_GPS::m_GPS_sig;
IMU_signal_t RTK_GPS::m_IMU_sig;
MAG_signal_t RTK_GPS::m_MAG_sig;
NED_signal_t RTK_GPS::m_NED_sig;
BaselineNED_signal_t RTK_GPS::m_BaselineNED_sig;

