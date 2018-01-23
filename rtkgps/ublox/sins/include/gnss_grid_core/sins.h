#ifndef  GNSS_SINS_H_
#define  GNSS_SINS_H_

#include <thread>

#define CPI (3.141592653589793)
#define DEG2RAD(x) ((x)*CPI/180)
#define RAD2DEG(x) ((x)*180/CPI)


#pragma pack(1)
struct UbxNavPvt
{
  uint8_t  Header_A;
  uint8_t  Header_B;
  uint8_t  Class;
  uint8_t  ID;
  uint16_t length;
  uint32_t iTOW;
  uint16_t year;
  uint8_t  month;
  uint8_t  day;
  uint8_t  hour;
  uint8_t  min;
  uint8_t  sec;
  uint8_t  valid;
  uint32_t tAcc;
  int32_t  nano;
  uint8_t  fixType;
  uint8_t  flags;
  uint8_t  flags2;
  uint8_t  numSV;
  int32_t  lon;
  int32_t  lat;
  int32_t  height;
  int32_t  hMSL;
  uint32_t hAcc;
  uint32_t vAcc;
  int32_t  velN;
  int32_t  velE;
  int32_t  velD;
  int32_t  gSpeed;
  int32_t  headMot;
  uint32_t sAcc;
  uint32_t headAcc;
  uint16_t pDOP;
  uint8_t  reserved1[6];
  int32_t  headVeh;
  int16_t  magDec;
  uint16_t magAcc;
  uint8_t  CK_A;
  uint8_t  CK_B;
  float    lon_deg_f;
  float    lat_deg_f;
  float    lon_rad_f;
  float    lat_rad_f;
  float    height_m;
  float    hMSL_m;
  float    hAcc_m;
  float    vAcc_m;
  float    velN_mps;
  float    velE_mps;
  float    velD_mps;
  float    gSpeed_mps;
  float    headMot_deg_f;
  float    headMot_rad_f;
  float    sAcc_mps;
  float    headAcc_rad_f;
  float    pDOP_m;
};

struct UbxEslIns
{
  uint8_t  Header_A;
  uint8_t  Header_B;
  uint8_t  Class;
  uint8_t  ID;
  uint16_t length;
  uint32_t bitfield0;
  uint8_t  reserved1[4];
  uint32_t iTOW;
  int32_t  gyro_x;
  int32_t  gyro_y;
  int32_t  gyro_z;
  int32_t  Accel_x;
  int32_t  Accel_y;
  int32_t  Accel_z;
  uint8_t  CK_A;
  uint8_t  CK_B;
  float    gyro_x_dps;
  float    gyro_y_dps;
  float    gyro_z_dps;
  float    Accel_x_g;
  float    Accel_y_g;
  float    Accel_z_g;
};
#pragma pack()

#endif