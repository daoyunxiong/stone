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
	sprintf(cmd, "gpsd -F /var/run/gpsd_pos.sock  %s -S 2900", port_pos);
	system(cmd);
	usleep(500000);
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
	pthread_rwlock_rdlock(&rwlock);
	rtk_data_t rtk_data_buf = rtk_data;
	pthread_rwlock_unlock(&rwlock);
	return rtk_data_buf;
}

rtk_aux_msg RtkReading::get_rtk_aux_data() {
	pthread_rwlock_rdlock(&rwlock);
	rtk_aux_msg rtk_data_buf = this->rtk_aux_data;
	pthread_rwlock_unlock(&rwlock);
	return rtk_data_buf;
}

int RtkReading::set_rtk_data(int flag) {
	gpsmm* gps_rec;
	if(0 == flag ){
		static gpsmm gps_rec_pos("localhost", "2900");
		gps_rec = &gps_rec_pos;
		if (gps_rec->stream(WATCH_ENABLE | WATCH_JSON ) == NULL) {
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
		if (!gps_rec->waiting(gpsd_wait_time * 2 ))
			continue;
		if ((gpsd_data = gps_rec->read()) == NULL) {
			std::cerr << "Read error.\n";
			return 0;
		} else {
			pthread_rwlock_wrlock(&rwlock);
			if(0 == flag){	
				rtk_data.local_time = getCurrentTicks();
				rtk_data.gps_time = gpsd_data->fix.time;
				rtk_data.latitude = gpsd_data->fix.latitude;
				rtk_data.longitude = gpsd_data->fix.longitude;
				rtk_data.altitude = gpsd_data->fix.altitude;
				UTMCoor utm;
				LatLonToUTMXY(gpsd_data->fix.latitude, gpsd_data->fix.longitude, 0, &utm);
				rtk_data.utmx = utm.x;
				rtk_data.utmy = utm.y;
				rtk_data.pos_status = gpsd_data->fix.mode;
		//		std::cout << gpsd_data->status << std::endl;
		//		std::cout << gpsd_data->fix.mode << std::endl;
				if(!isnan(gpsd_data->fix.track))
					rtk_data.reference_heading = gpsd_data->fix.track;

				rtk_data.attitude_heading = gpsd_data->attitude.heading;
				if(gpsd_data->attitude.heading < 0.000001)
					rtk_data.heading_status = 0;
				else
					rtk_data.heading_status = 4;

				rtk_aux_data.satelites_num = gpsd_data->satellites_visible;
				double sn_data = 0;
				int count = 0;
				for(int i = 0; i<24; i++)
				{
					if(gpsd_data->skyview[i].PRN != 0){
						sn_data += gpsd_data->skyview[i].ss;
						count ++;
		//			std::cout << gpsd_data->skyview[i].PRN << " " 
	//				<< gpsd_data->skyview[i].ss << " " 
	//				<< gpsd_data->skyview[i].used << " " 
	//				<< gpsd_data->skyview[i].elevation << " " 
	//				<< gpsd_data->skyview[i].azimuth << " " 
	//				<< gpsd_data->satellites_used << " " 
	//				<< gpsd_data->satellites_visible << " " 
	//				<< std::endl;
					}
				}
				if(count != 0)
					rtk_aux_data.sn_ratio = sn_data / count;
			}
			else if(1 == flag){
				rtk_data.attitude_heading = gpsd_data->attitude.heading;
				if(gpsd_data->attitude.heading < 0.000001)
					rtk_data.heading_status = 4;
				else
					rtk_data.heading_status = gpsd_data->fix.mode;
			}
			pthread_rwlock_unlock(&rwlock);
		}
	}
}
