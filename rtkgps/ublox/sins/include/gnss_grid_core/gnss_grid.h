#ifndef  GNSS_GRID_H_
#define  GNSS_GRID_H_

#include <thread>
#include "serial.h"
#include "circle_buffer.h"
#include "sins.h"

// System Headers
#include <functional>
#include <memory>
#include <iostream>
#include <string>

// tjh Headers
#include <tjhPublisher/Publisher.h>

namespace PIRoBot {

class GnssInfo
{
public:
    GnssInfo(int32_t gridx, int32_t gridy, double lat, double lon):
        m_grid_x(gridx),
        m_grid_y(gridy),
        m_lat(lat),
        m_lon(lon)
    {

    }
public:
    int32_t m_grid_x;
    int32_t m_grid_y;

    double m_lat;
    double m_lon;
};

class Gnss_grid : public tjh::IPublisher<const GnssInfo&>  {
  public:
    Gnss_grid();
    ~Gnss_grid();
    bool Init(std::string serialName);
    bool start();
    void Finish();
    bool isStopped();
    void RunOnce();
    
    void set_origin(double x_lat,double y_lon,uint8_t type);
    void get_target_gnss_grid(double lat,double lon,int32_t *grid_x,int32_t *grid_y);
    void get_gnss_grid(int32_t *x,int32_t *y);
    void get_gyro(float *x,float *y,float *z);
    void get_accel(float *x,float *y,float *z);
    void get_signal_quality(float *sq,float *hdop);
    void get_pvt(struct UbxNavPvt *p);

  private:
    double  orign_x;
    double  orign_y;
    int32_t grid_x;
    int32_t grid_y;

    uint8_t flag_static;
    uint8_t flag_static_bak;
    uint8_t flag_output;

    Serial serial;
    CircleBuffer circleBuf;
    struct UbxNavPvt pvt;
    struct UbxEslIns ins;

    bool checksum(uint8_t *buffer,uint32_t len);
    bool AnalysisPackage(CircleBuffer * circleBuf, struct UbxNavPvt *pvt, struct UbxEslIns *ins);
    void ProcessPackageData(struct UbxNavPvt *pvt, struct UbxEslIns *ins); 
    void gnss_grid_calc(double lat,double lon,int32_t *grid_x,int32_t *grid_y); 

public:

    void mainLoop();   
    virtual void subscribe(std::weak_ptr<std::function<void(const GnssInfo&)> > callback);
private:
    bool mbForceStopThread;
    bool mbStopped;
    std::thread* mpMThread;
   
   tjh::Publisher<Gnss_grid, const GnssInfo&> _publisher;
   void GnssUpdated(const GnssInfo& gnss);
};

}
#endif

