#include <glog/logging.h>
#include <gflags/gflags.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <sys/time.h> // for gettimeofday
#include <iomanip>
#include "serial.h"
#include "circle_buffer.h"
#include "sins.h"
#include "codegen_main.h"
#include <chrono>

#define OutputSerialRead 1
#define OutputubxRead 1

DEFINE_string(serialName, "/dev/ttyACM0", "serial port name");

using namespace std;

static bool AnalysisPackage(CircleBuffer * circleBuf, struct UbxNavPvt *pvt);
static void ProcessPackageData(struct UbxNavPvt *pvt);


#define xm_origin_x 2494563.7
#define xm_origin_y 38493931.7
#define szu_origin_x 2493429.768
#define szu_origin_y 38493408.335

static double orign_x = szu_origin_x;
static double orign_y = szu_origin_y;
static int32_t grid_x = 0;
static int32_t grid_y = 0;

void set_origin(double x,double y)
{
  orign_x = x;
  orign_y = y;
}

static bool checksum(uint8_t *buffer,uint32_t len)
{
  uint8_t CK_A=0;
  uint8_t CK_B=0;
  uint8_t i=0;

  for(i=0;i<len-2;i++)
  {
    CK_A = CK_A + buffer[i];
    CK_B = CK_B +CK_A;
  }

  if(CK_A==buffer[len-2] && CK_B==buffer[len-1])
  {
    return true;
  }
  else
  {
    return false;
  }
}


// ./ublox_reader --log_dir=. --v=1
int main(int argc, char *argv[])
{
  FLAGS_log_dir = "./log";
  google::InitGoogleLogging(argv[0]);
  google::ParseCommandLineFlags(&argc, &argv, false);
  google::InstallFailureSignalHandler();
  codegen_main();

  LOG(INFO) << "I am INFO!";
  LOG(WARNING) << "I am WARNING!";
  VLOG(1) << " I am vlog";
 // LOG(ERROR) << "I am ERROR!";
 // LOG(FATAL) << "I am FATAL!";

  Serial ubloxSerial;
  bool ret = ubloxSerial.Open(FLAGS_serialName.c_str());
  if(!ret) {
    std::cout<<"open failed"<<std::endl;
    return -1;
  }

  VLOG(1) << "FINE";
  unsigned char buf[1024];
  int readSize;
  //static unsigned char dataBuf[1024];
  static struct UbxNavPvt pvt;
  CircleBuffer circleBuf(1024);
  while(1) 
  {
 //   sleep(1);
 //   printf("ok \n");
    ubloxSerial.Read(buf, &readSize);

    if(readSize > 0){
    //buf[readSize] = '\0';
    char *start;
 //   start = memchr(buf, '$', 100);
 //   if(buf[0] == '$' && buf[1] == 'G' && buf[2] == 'N' && buf[3] == 'G' && buf[4] == 'G'  )
 //     printf("the gga %s",buf);
    }
  //  cout << *start << endl; 
//    std::cout << buf << endl;
 //   if(! readSize)
  //    continue; 
  //  std::cout << readSize << std::endl;
//    readSize = 1;
  //  buf[0]='1';
  // cout << (int)buf[0] << endl;
		#ifdef OutputSerialRead
    std::stringstream ss;
    for(int i=0; i<readSize; i++) {
        ss<<std::hex<<"0x"<<std::setfill('0')<<std::setw(2)<<(int)buf[i]<<" ";
    // cout << (int)buf[i] << endl;
   // printf("the num %x \n", buf[i]);
    }
   // cout << ss.str() << endl;
    if(readSize > 0) {
      VLOG(1)<<"read: "<<ss.str();
    }
		#endif    
		
    circleBuf.Push(buf, readSize);
    int32_t dataLen;
    if(AnalysisPackage(&circleBuf, &pvt)) {
      ProcessPackageData(&pvt);
    }
  }
}


static bool AnalysisPackage(CircleBuffer * circleBuf, struct UbxNavPvt *pvt) 
{
  if(circleBuf->DataLength() >= 100) 
  {
    unsigned char data[1024];
    int lens = circleBuf->DataLength();
    circleBuf->Pop(data,lens);
    
 //  cout << "this " << endl;
    if (lens==100 && data[0] == 0xb5 && data[1] == 0x62 && data[2] == 0x01 && data[3] == 0x07)
    {
      if(checksum(&data[2],lens-2))
      {
        double x,y;
        double delta_x,delta_y;

        memcpy(&pvt->Header_A,data,lens);
        
        pvt->lon_deg_f = pvt->lon/10000000.0;
        pvt->lat_deg_f = pvt->lat/10000000.0;
        pvt->lon_rad_f = DEG2RAD(pvt->lon_deg_f);
        pvt->lat_rad_f = DEG2RAD(pvt->lat_deg_f);
        pvt->height_m  = pvt->height/1000.0;
        pvt->hMSL_m    = pvt->hMSL/1000.0;
        pvt->hAcc_m    = pvt->hAcc/1000.0;
        pvt->vAcc_m    = pvt->vAcc/1000.0;
        pvt->velN_mps  = pvt->velN/1000.0;
        pvt->velE_mps  = pvt->velE/1000.0;
        pvt->velD_mps  = pvt->velD/1000.0;
        pvt->gSpeed_mps= pvt->gSpeed/1000.0;
        pvt->headMot_deg_f = pvt->headMot/100000.0;
        pvt->headMot_rad_f = DEG2RAD(pvt->headMot_deg_f);
        pvt->sAcc_mps  = pvt->sAcc/1000.0;
        pvt->headAcc_rad_f = pvt->headAcc/100000.0;
        pvt->pDOP_m = pvt->pDOP/100.0;


				main_Gauss_projection_Positive(pvt->lat_deg_f,pvt->lon_deg_f,3, &x, &y);
				delta_x = x-orign_x;
				delta_y = y-orign_y;
			  grid_x = (int32_t)(delta_y);
				grid_y = (int32_t)(delta_x);


       // std::cout<<delta_x<<","<<delta_y<<","<<grid_x<<","<<grid_y<<std::endl;

        #ifdef OutputubxRead
        std::stringstream ss;
        ss<<pvt->iTOW<<",";
        ss<<pvt->year<<",";
        ss<<(int)pvt->month<<",";
        ss<<(int)pvt->day<<",";
        ss<<(int)pvt->hour<<",";
        ss<<(int)pvt->min<<",";
        ss<<(int)pvt->sec<<",";
        ss<<(int)pvt->valid<<",";
        ss<<pvt->tAcc<<",";
        ss<<pvt->nano<<",";
        ss<<(int)pvt->fixType<<",";
        ss<<(int)pvt->flags<<",";
        ss<<(int)pvt->flags2<<",";
        ss<<(int)pvt->numSV<<",";
        ss<<pvt->lon<<",";
        ss<<pvt->lat<<",";
        ss<<pvt->height<<",";
        ss<<pvt->hMSL<<",";
        ss<<pvt->hAcc<<",";
        ss<<pvt->vAcc<<",";
        ss<<pvt->velN<<",";
        ss<<pvt->velE<<",";
        ss<<pvt->velD<<",";
        ss<<pvt->gSpeed<<",";
        ss<<pvt->headMot<<",";
        ss<<pvt->sAcc<<",";
        ss<<pvt->headAcc<<",";
        ss<<pvt->pDOP<<",";
        ss<<pvt->headVeh<<",";
        ss<<x<<",";
        ss<<y<<",";
        ss<<delta_x<<",";
        ss<<delta_y<<",";
        ss<<grid_x<<",";
        ss<<grid_y;

cout<< "tow:" << pvt->iTOW << endl;
cout<< "lon:" << pvt->lon << endl;
cout << "lat:" << pvt->lat << endl;
//        cout << "height" << pvt->height << endl;
cout << "pdop" << pvt->pDOP << endl;
cout << "ftype" << (int)pvt->fixType << endl;
        VLOG(1)<<"read: "<<ss.str();
        #endif

        return true;
      }
      
    }
  }
  return false;
}

void get_gnss_grid(int32_t *x,int32_t *y)
{
  *x = grid_x;
  *y = grid_y;	
}


static void ProcessPackageData(struct UbxNavPvt *pvt) 
{


}

