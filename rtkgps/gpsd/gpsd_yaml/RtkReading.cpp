/*************************************************************************
  > File Name: RtkReading.cpp
  > Author: ma6174
  > Mail: ma6174@163.com
  > Created Time: Wed 13 Jun 2018 02:33:31 PM CST
 ************************************************************************/

#include "RtkReading.h"
using namespace std;

RtkReading::RtkReading(char* file_path)
	: threadActive(true) {
		YAML::Node global_config = YAML::LoadFile(file_path);
		antenna_num = global_config["debug"]["antenna_num"].as<int>();
		rtk_data.gps_mode = global_config["debug"]["antenna_num"].as<int>();
		gpsd_wait_time = 1000000 / global_config["debug"]["refresh_frequency"].as<int>();
		port_num = global_config["debug"]["port_num"].as<int>();
		char* port_pos = (char*) global_config["debug"]["port_pos"].as<std::string>().c_str();
		char* port_heading = (char*) global_config["debug"]["port_heading"].as<std::string>().c_str();
		threadActive = true;
		rwlock = PTHREAD_RWLOCK_INITIALIZER;
		init_gpsd(port_pos, port_heading);
		Gps_thread_pos = new std::thread(std::bind(&RtkReading::set_rtk_data, this, 0));
		if(2 == port_num)
			Gps_thread_heading = new std::thread(std::bind(&RtkReading::set_rtk_data, this, 1));
	}

RtkReading::~RtkReading() {
	threadActive = false;
	if (Gps_thread_pos->joinable())
		Gps_thread_pos->join();
	delete Gps_thread_pos;
	if(2 == port_num){
		if(Gps_thread_heading->joinable())
			Gps_thread_heading->join();
		delete Gps_thread_heading;
	}
	close_gpsd();
	cout << threadActive << endl;

	cout << "exit ok" << endl;
}

double RtkReading::getCurrentTicks() {
	timeval t;
	gettimeofday(&t,NULL);
	uint64_t ms_data = static_cast<uint64_t>( t.tv_sec ) * 1000ull + static_cast<uint64_t>( t.tv_usec ) / 1000;
	return (double) ms_data / 1000;
}

int RtkReading::init_gpsd(char *port_pos, char *port_heading) {

	char cmd[100];
	memset(cmd, 0, sizeof(cmd));
	sprintf(cmd, "gpsd %s  -F /var/run/gpsd_pos.sock -S 2900", port_pos);
	system(cmd);
	usleep(500000);
	cout << "init" << endl;
	if(2 == port_num){
		memset(cmd, 0 , sizeof(cmd));
		sprintf(cmd, "gpsd %s -n -F /var/run/gpsd_heading.sock -S 2901", port_heading);
		system(cmd);
		usleep(500000);
	}
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
	set_rtk_data(0);
}
void RtkReading::process_heading(int flag) {
	cout << "begin process2" << endl;
	set_rtk_data(1);
}

int RtkReading::set_rtk_data(int flag) {
	gpsmm* gps_rec;
	if(0 == flag ){
		static gpsmm gps_rec_pos("localhost", "2900");
		gps_rec = &gps_rec_pos;
		if (gps_rec->stream(WATCH_ENABLE | WATCH_JSON) == NULL) {
			std::cerr << "No GPSD running.\n";
			return 0;
		}}
	else if(1 == flag ){
		static gpsmm gps_rec_heading("localhost", "2901");
		gps_rec = &gps_rec_heading;
		if (gps_rec->stream(WATCH_ENABLE | WATCH_JSON) == NULL) {
			std::cerr << "No GPSD running.\n";
			return 0;
		}}
	while (threadActive) {
		struct gps_data_t *gpsd_data;
		//	struct gps_data_t *newdata_head;
		if (!gps_rec->waiting(gpsd_wait_time / 2))
			continue;
		if ((gpsd_data = gps_rec->read()) == NULL) {
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
					i	"," << endl;*/
			pthread_rwlock_wrlock(&rwlock);
			if(0 == flag){	
				rtk_data.local_time = getCurrentTicks();
				rtk_data.gps_time = gpsd_data->fix.time;
				rtk_data.latitude = gpsd_data->fix.latitude;
				rtk_data.longitude = gpsd_data->fix.longitude;
				rtk_data.altitude = gpsd_data->fix.altitude;
				LatLonToUTMXY(gpsd_data->fix.latitude, gpsd_data->fix.longitude, 0, &rtk_data.utm);
				rtk_data.utmx = rtk_data.utm.x;
				rtk_data.utmy = rtk_data.utm.y;
				rtk_data.pos_status = gpsd_data->status;
				rtk_data.reference_heading = gpsd_data->fix.track;
			}
			else if(1 == flag){
				rtk_data.attitude_heading = gpsd_data->attitude.heading;
				//std::cout << gpsd_data->attitude.heading << std::endl;
			}
			pthread_rwlock_unlock(&rwlock);
		}
	}
}

int RtkReading::set_rtk_heading_data(int flag) {
	if(1 == flag)
	{

		gpsmm gps_rec("localhost", "2901");
		if (gps_rec.stream(WATCH_ENABLE | WATCH_JSON) == NULL) {
			std::cerr << "No GPSD running.\n";
			return 0;
		}
		while (threadActive) {
			struct gps_data_t *gpsd_data;
			if (!gps_rec.waiting(200000))
				continue;
			if ((gpsd_data = gps_rec.read()) == NULL) {
				std::cerr << "Read error.\n";
				return 0;
			} else {
				pthread_rwlock_wrlock(&rwlock);
				if(0 == flag){	
					rtk_data.local_time = gpsd_data->fix.time;
					rtk_data.status = gpsd_data->status;
					rtk_data.llh_t.lat = gpsd_data->fix.latitude;
					rtk_data.llh_t.log = gpsd_data->fix.longitude;
					LatLonToUTMXY(gpsd_data->fix.latitude, gpsd_data->fix.longitude, 0, &rtk_data.utm);

					std::cout << std::setprecision(12);
					std::cout << gpsd_data->fix.time <<" pos"<< std::endl;
				}
				else if(1 == flag){
					rtk_data.attitude_heading = gpsd_data->attitude.heading;
					//std::cout << gpsd_data->fix.time << " heading" <<std::endl;
				}
				pthread_rwlock_unlock(&rwlock);
			}
		}

	}
	return 0;
}
