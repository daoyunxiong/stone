/********************************************
 *  Gpsd client example
 *  created by s.farmbauer@red-beard-code.de
 ********************************************/
#include <chrono>
#include <libgpsmm.h>
#include "GpsPosition.h"
#include "MutexLocker.h"
#include <iostream>


GPSPosition GPSPosition::m_slast_3d_value(NULL);
GPSPosition GPSPosition::m_slast_2d_value(NULL);
std::mutex GPSPosition::m_slock;

GPSPosition::GPSPosition(gps_data_t *data)
        :m_dlatitude(0)
        ,m_dlongitude(0)
        ,m_daltitude(0)
        ,m_mode(GPSMODE_NO_VALID_DATA)

{
    if(data == NULL || !(data->set & PACKET_SET))
    {
        return;
    }
    m_mode = GpsMode(data->fix.mode);
    m_dlatitude = data->fix.latitude;
    m_dlongitude = data->fix.longitude;
    m_daltitude = data->fix.altitude;
    m_time = data->fix.time;
    m_track = data->fix.track;
    m_speed = data->fix.speed;

    std::chrono::high_resolution_clock::time_point epoch = std::chrono::time_point<std::chrono::high_resolution_clock>();
    uint64_t seconds = uint64_t(data->fix.time);
    uint64_t microseconds = uint64_t((data->fix.time-seconds)*1e6);
    std::chrono::high_resolution_clock::duration us_since = std::chrono::seconds(seconds)+ std::chrono::microseconds(microseconds);
    m_timepoint = epoch + us_since;
    {
        MutexLocker locker(m_slock);
        if (m_mode == GPSPosition::GPSMODE_3D_FIX) {
            m_slast_3d_value = *this;
        } else if (m_mode == GPSPosition::GPSMODE_2D_FIX) {
            m_slast_2d_value = *this;
        }
    }

}

GPSPosition GPSPosition::get_last_3d_fix_value()
{
    MutexLocker locker(m_slock);
    return m_slast_3d_value;
}

GPSPosition GPSPosition::get_last_2d_fix_value()
{
    MutexLocker locker(m_slock);
    return m_slast_2d_value;
}

