/********************************************
 *  Gpsd client example
 *  created by s.farmbauer@red-beard-code.de
 ********************************************/

#include <libgpsmm.h>
#include <iostream>
#include <iomanip>
#include <thread>
#include "GpsPosition.h"

using namespace std;

//Controlling the thread flow
bool gReadGps = true;



/// Thread which reads all position form the gpsd
/// server
void read_gpsd_data() {
    gpsmm gps_rec("localhost", DEFAULT_GPSD_PORT);

    if (gps_rec.stream(WATCH_ENABLE|WATCH_JSON) == NULL) {
        std::cerr << "No GPSD running.\n";
        return;
    }

    while (gReadGps) {
        struct gps_data_t *newdata;

        if (!gps_rec.waiting(2000000))
            continue;

        if ((newdata = gps_rec.read()) == NULL) {
            std::cerr << "Read error.\n";
            return;
        } else {
            GPSPosition dummy(newdata);

        }
    }
        
}

///Starts the thread which reads the position and stores
///it in a Contaier
int main(void)
{
    std::thread gps_thread(read_gpsd_data);
    for (int i=0; i<10000; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        GPSPosition last_pos = GPSPosition::get_last_3d_fix_value();
        std::cout << "Mode:" << last_pos.get_mode() 
                  << " Lat: " << std::setprecision(10) << last_pos.get_latitude()
                  << " Lon: " << std::setprecision(10) << last_pos.get_longitude()
                  << " Alt: " << std::setprecision(10) << last_pos.get_altitude()
                  << " heading: " << std::setprecision(10) << last_pos.get_track()
                  << " speed: " << std::setprecision(10) << last_pos.get_speed()
                  << " time: " <<std::setprecision(11) << last_pos.get_gpstime() 
                  << std::endl;
     }
    gReadGps = false;
    gps_thread.join();
    return 0;
}
