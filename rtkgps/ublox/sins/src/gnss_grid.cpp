#include <glog/logging.h>
#include <gflags/gflags.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <sys/time.h> // for gettimeofday
#include <iomanip>
#include "serial.h"
#include "circle_buffer.h"
#include "gnss_grid.h"
#include "codegen_main.h"
#include <chrono>

//#define OutputSerialRead
// #define OutputubxRead

using namespace std;

namespace PIRoBot {

Gnss_grid::Gnss_grid():circleBuf(1024), mbForceStopThread(false), mpMThread(NULL)
{
  codegen_main();
  orign_x = 0;
  orign_y = 0;
  grid_x = 0;
  grid_y = 0;

  flag_static = 0;
  flag_static_bak = 0;
  flag_output = 0;
}

Gnss_grid::~Gnss_grid()
{
  delete mpMThread;
}

bool Gnss_grid::Init(std::string serialName)
{
  bool ret = serial.Open(serialName.c_str());
  
  if(!ret) {
    std::cout<<"open failed"<<std::endl;
  }

  return ret;
}

void Gnss_grid::Finish() {
  mbForceStopThread = true;
  serial.Close();
}

bool Gnss_grid::isStopped()
{
  return mbStopped;
}

void Gnss_grid::set_origin(double x_lat,double y_lon,uint8_t type)
{
  double px,py;
  if (type==0)
  {
    main_Gauss_projection_Positive(x_lat,y_lon,3, &px, &py);
  }
  else
  {
    px = x_lat;
    py = y_lon;
  }

  orign_x = px;
  orign_y = py;
}

void Gnss_grid::get_gnss_grid(int32_t *x,int32_t *y)
{
  *x = grid_x;
  *y = grid_y;	
}

void Gnss_grid::get_gyro(float *x,float *y,float *z)
{
  *x = ins.gyro_x_dps;
  *y = ins.gyro_y_dps;
  *z = ins.gyro_z_dps;
}

void Gnss_grid::get_accel(float *x,float *y,float *z)
{
  *x = ins.Accel_x_g;
  *y = ins.Accel_y_g;
  *z = ins.Accel_z_g;
}

void Gnss_grid::get_signal_quality(float *sq,float *hdop)
{
  if(pvt.numSV>=8 && pvt.fixType>2)
  {   
    float tmp1 = 1.0/pvt.hAcc_m;
    if (tmp1>1.0) tmp1 =1.0;
    *sq = tmp1;
  }
  else
  {
    *sq = 0.0;
  }

  *hdop = pvt.hAcc_m;
}

void Gnss_grid::get_pvt(struct UbxNavPvt *p)
{
  memcpy(&p->Header_A,&pvt.Header_A,sizeof(struct UbxNavPvt));
}

bool Gnss_grid::checksum(uint8_t *buffer,uint32_t len)
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


bool Gnss_grid::AnalysisPackage(CircleBuffer * circleBuf, struct UbxNavPvt *pvt, struct UbxEslIns *ins) 
{
  // printf("circleBuf len:%d\n", circleBuf->DataLength());

  if(circleBuf->DataLength() > 0) 
  {
    unsigned char data[1024];
    int lens = circleBuf->DataLength();
    circleBuf->Pop(data,lens);
    
    if (lens==44 && data[0] == 0xb5 && data[1] == 0x62 && data[2] == 0x10 && data[3] == 0x15)
    {
      if(checksum(&data[2],lens-2))
      {
        memcpy(&ins->Header_A,data,lens);
        ins->gyro_x_dps = ins->gyro_x/1000.0;
        ins->gyro_y_dps = ins->gyro_y/1000.0;
        ins->gyro_z_dps = ins->gyro_z/1000.0;
        ins->Accel_x_g  = ins->Accel_x/1000.0;
        ins->Accel_y_g  = ins->Accel_y/1000.0;
        ins->Accel_z_g  = ins->Accel_z/1000.0;
        std::cout<<ins->gyro_x<<","<<ins->gyro_y<<","<<ins->gyro_z<<std::endl;
        std::cout<<ins->Accel_x<<","<<ins->Accel_y<<","<<ins->Accel_z<<std::endl;
      }
    }

    if (lens==100 && data[0] == 0xb5 && data[1] == 0x62 && data[2] == 0x01 && data[3] == 0x07)
    {
      if(checksum(&data[2],lens-2))
      {
        double x,y;
        double delta_x,delta_y;

        // std::cout<<"receive ok"<<std::endl;

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


	    //detect motion state 
        if(pvt->gSpeed<50) // && pvt->numSV>10 && pvt->fixType>2)
        {
	        flag_static = 1;
        }
        else
        {
            flag_static = 0;
        }

        if ((flag_static==1 && flag_static_bak==0) || (flag_static==0))
        {
            flag_output=1;
        }
        else
        {
            flag_output=0;
        }

	    flag_static_bak = flag_static;


	    //std::cout<<(int)pvt->numSV<<","<<(int)pvt->fixType<<","<<pvt->hAcc_m<<","<<pvt->pDOP_m<<std::endl;

/*
        // 22.5487155,113.941017166667 --> 2494563.13743799 38493933.0142975
        main_Gauss_projection_Positive(22.5487155,113.941017166667,3, &x, &y);
        std::cout<<x<<","<<y<<std::endl;

	    // 22.5487071,113.9410090 --> 2494562.20757217 38493932.1739051
        main_Gauss_projection_Positive(22.5487071,113.9410090,3, &x, &y);
        delta_x = x-orign_x;
        delta_y = y-orign_y;
        grid_x = (int32_t)(delta_y);
        grid_y = (int32_t)(delta_x);
        std::cout<<setprecision(16)<<x<<","<<y<<","<<delta_x<<","<<delta_y<<","<<grid_x<<","<<grid_y<<std::endl;
*/
	    //std::cout<<pvt->lat_deg_f<<","<<pvt->lon_deg_f<<std::endl;

        main_Gauss_projection_Positive(pvt->lat_deg_f,pvt->lon_deg_f,3, &x, &y);
        delta_x = x-orign_x;
        delta_y = y-orign_y;
        grid_x = (int32_t)(delta_y);
        grid_y = (int32_t)(delta_x);


        if(pvt->numSV>10 && pvt->fixType>2 && pvt->hAcc_m<2.0)// && pvt->pDOP_m<3.0)
        {
            //grid_x = (int32_t)(delta_y);
            //grid_y = (int32_t)(delta_x);
        }


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
        ss<<grid_y<<",";
        ss<<flag_static<<",";
        ss<<flag_output;


        VLOG(1)<<"read: "<<ss.str();
        #endif

        return true;
      }
      
    }
  }
  return false;
}


void Gnss_grid::ProcessPackageData(struct UbxNavPvt *pvt, struct UbxEslIns *ins) 
{


}


void Gnss_grid::RunOnce() {
  unsigned char buf[1024];
  int readSize;
  serial.Read(buf, &readSize);
  circleBuf.Push(buf, readSize);

  // printf("runonce, readsize:%d\n", readSize);

#ifdef OutputSerialRead
  std::stringstream ss;
  for(int i=0; i<readSize; i++) {
      ss<<std::hex<<"0x"<<std::setfill('0')<<std::setw(2)<<(int)buf[i]<<" ";
  }
  if(readSize > 0) {
    VLOG(1)<<"read: "<<ss.str();
  }
#endif 

  // Note: dataBuf must static, bcz may call AnalysisPackage multiple times to get a complete package.
  if(AnalysisPackage(&circleBuf, &pvt, &ins)) {
    ProcessPackageData(&pvt, &ins);
  }
}

void Gnss_grid::mainLoop()
{
  GnssInfo gnss(grid_x, grid_y, 0, 0);
  static int32_t old_gx, old_gy;

  while(!mbForceStopThread)
  {
    old_gx = grid_x;
    old_gy = grid_y;    

    RunOnce();

    // if grid index changed, notify others
    if(old_gx != grid_x || old_gy != grid_y)
    {
      gnss.m_grid_x = grid_x;
      gnss.m_grid_y = grid_y;
      gnss.m_lon = pvt.lon_deg_f;
      gnss.m_lat = pvt.lat_deg_f;
      GnssUpdated(gnss);
    }

    // delay 10ms
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    cout << gnss.m_lat << endl;
    cout << gnss.m_lon << endl;
 
  }

  mbStopped = true;
}

bool Gnss_grid::start()
{
  if(mpMThread)
  {
    mpMThread->detach();
    delete mpMThread;
  }

  mbForceStopThread = false;
  mbStopped = false;

  mpMThread = new std::thread(&Gnss_grid::mainLoop, this);

  return true;
}


void Gnss_grid::gnss_grid_calc(double lat,double lon,int32_t *grid_x,int32_t *grid_y)
{
  double px,py;
  double delta_x,delta_y;
  main_Gauss_projection_Positive(lat,lon,3, &px, &py);
  delta_x = px-orign_x;
  delta_y = py-orign_y;
  *grid_x = (int32_t)(delta_y);
  *grid_y = (int32_t)(delta_x);
}


void Gnss_grid::get_target_gnss_grid(double lat,double lon,int32_t *grid_x,int32_t *grid_y)
{
  gnss_grid_calc(lat,lon,grid_x,grid_y);
}

void Gnss_grid::GnssUpdated(const GnssInfo& gnss)
{
  // Publish the contents of a and b to any listeners.
  _publisher.fire(gnss);
}

void Gnss_grid::subscribe(std::weak_ptr<std::function<void(const GnssInfo&)> > callback)
{
    // This is a simple pass through to the composed publisher.
    _publisher.subscribe(callback);
}

}//end of namespace PIRoBot


