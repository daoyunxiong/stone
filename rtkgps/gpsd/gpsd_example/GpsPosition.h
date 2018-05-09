/********************************************
 *  Gpsd client example
 *  created by s.farmbauer@red-beard-code.de
 ********************************************/

#ifndef GPSD_EXAMPLE_GPSPOSITION_H
#define GPSD_EXAMPLE_GPSPOSITION_H

#include <mutex>

///Class for easy handling of gps position.
///The constructor gets a gps_data_t struct and copys the data
///into the class members.
///If the data has a fix, it will be stored in a static member,
///which is protected with a mutex to make it thread safe.
///So you can always get the last position with an gps fix.

class GPSPosition
{
public:
    enum GpsMode{GPSMODE_NO_VALID_DATA, GPSMODE_NO_FIX, GPSMODE_2D_FIX, GPSMODE_3D_FIX};
    GPSPosition(gps_data_t* data);
    double get_latitude() const { return m_dlatitude; };
    double get_longitude() const { return m_dlongitude; };
    double get_altitude() const { return m_daltitude; };
    double get_gpstime() const { return m_time; };
    double get_track() const { return m_track; };
    double get_speed() const { return m_speed; };
    std::chrono::high_resolution_clock::time_point get_time() const { return m_timepoint; };
    GpsMode get_mode()const { return m_mode; };
    static GPSPosition get_last_3d_fix_value();
    static GPSPosition get_last_2d_fix_value();

private:
    double m_dlatitude;
    double m_dlongitude;
    double m_daltitude;
    double m_time;
    double m_track;
    double m_speed;
    std::chrono::high_resolution_clock::time_point m_timepoint;
    GpsMode m_mode;
    static GPSPosition m_slast_3d_value;
    static GPSPosition m_slast_2d_value;
    static std::mutex m_slock;
};

#endif //GPSD_EXAMPLE_GPSPOSITION_H
