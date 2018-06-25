/*************************************************************************
  > File Name: RtkReading.cpp
  > Author: ma6174
  > Mail: ma6174@163.com
  > Created Time: Wed 13 Jun 2018 02:33:31 PM CST
 ************************************************************************/

#include "RtkReading.h"
using namespace std;

RtkReading::RtkReading(char *port_pos, char *port_heading)
	: threadActive(true) {
		threadActive = true;
		rwlock = PTHREAD_RWLOCK_INITIALIZER;
		init_gpsd(port_pos, port_heading);
		Gps_thread_pos = new std::thread(std::bind(&RtkReading::process, this, 0));
		Gps_thread_heading = new std::thread(std::bind(&RtkReading::process, this, 1));
	}

RtkReading::~RtkReading() {
	threadActive = false;
	if (Gps_thread_pos->joinable())
		Gps_thread_pos->join();
	if(Gps_thread_heading->joinable())
		Gps_thread_heading->join();
	delete Gps_thread_pos;
    delete Gps_thread_heading;
	close_gpsd();
	cout << threadActive << endl;

	cout << "exit ok" << endl;
}

int RtkReading::init_gpsd(char *port_pos, char *port_heading) {
	char cmd[100];
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "gpsd %s  -F /var/run/gpsd_pos.sock -S 2900", port_pos);
	system(cmd);
	usleep(500000);
	cout << "init" << endl;
	//	memset(cmd, 0 , sizeof(cmd));
	//	sprintf(cmd, "gpsd %s -n -F /var/run/gpsd_heading.sock -S 2901",
	// port_heading);
	//	system(cmd);
}

int RtkReading::close_gpsd() {
	system("killall gpsd");
	usleep(1000000);
	cout << "close gpsd" << endl;
}

rtk_data_t RtkReading::get_rtk_data() {
	//       GPSPosition last_pos = GPSPosition::get_last_3d_fix_value();
	//		data.llh_t.lat = last_pos.get_latitude();
	//		data.llh_t.log = last_pos.get_longitude();
	//		data.status = last_pos.get_mode();
	pthread_rwlock_rdlock(&rwlock);
	rtk_data_t rtk_data_buf = rtk_data;
	pthread_rwlock_unlock(&rwlock);
	return rtk_data_buf;
}

void RtkReading::process(int flag) {
	cout << "begin process" << endl;
	set_rtk_data(flag);
}

int RtkReading::set_rtk_data(int flag) {

	cout << "begin read" << endl;
	char gps_port[5];
	if(0 == flag)
		strcpy(gps_port, "2900");
	else if(1 == flag)
		strcpy(gps_port, "2901");
	gpsmm gps_rec("localhost", gps_port);

	if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == NULL) {
		std::cerr << "No GPSD running.\n";
		return 0;
	}
	//    if (gps_rec_head.stream(WATCH_ENABLE|WATCH_JSON) == NULL) {
	//       std::cerr << "No GPSD running.\n";
	//       return 0;
	//   }

	while (threadActive) {
		struct gps_data_t *gpsd_data;
		//	struct gps_data_t *newdata_head;
		if (!gps_rec.waiting(200000))
			continue;

		if ((gpsd_data = gps_rec.read()) == NULL) {
			std::cerr << "Read error.\n";
			return 0;
		} else {
			//			while (((gpsd_data = gps_rec.read()) == NULL) ||
			//					(gpsd_data->fix.mode < MODE_2D))
			//{
			//			}
			/*		std::cout.precision(12);
					cout << "status:" << gpsd_data->status << ","
					<< "track:" << gpsd_data->fix.track i<< ","
					<< "speed:" << gpsd_data->fix.speed << ","
					<< "time:" << gpsd_data->fix.time << ","
					<< "mode:" << gpsd_data->fix.mode << ","
					<< "num:" << gpsd_data->satellites_used << ","
					<< "heading:" << gpsd_data->attitude.heading <<
					"," << endl;*/
			pthread_rwlock_wrlock(&rwlock);
			if(0 == flag){	
				rtk_data.local_time = gpsd_data->fix.time;
				rtk_data.status = gpsd_data->status;
				rtk_data.llh_t.lat = gpsd_data->fix.latitude;
				rtk_data.llh_t.log = gpsd_data->fix.longitude;
				LatLonToUTMXY(gpsd_data->fix.latitude, gpsd_data->fix.longitude, 0, &rtk_data.utm);
			}
			else if(1 == flag){
				rtk_data.dual_antenna_heading = gpsd_data->attitude.heading;
			}
			pthread_rwlock_unlock(&rwlock);
		}
	}
}
