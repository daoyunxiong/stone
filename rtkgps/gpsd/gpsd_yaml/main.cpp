/********************************************
 *  Gpsd client example
 *  created by s.farmbauer@red-beard-code.de
 ********************************************/
#include "RtkReading.h"
#include <iomanip>
#include <iostream>
#include <libgpsmm.h>
#include <signal.h>
#include <thread>

#include "yaml-cpp/emitter.h"
#include "yaml-cpp/node/emit.h"
#include "yaml-cpp/node/node.h"
#include "yaml-cpp/node/impl.h"
#include "yaml-cpp/node/convert.h"
#include "yaml-cpp/node/iterator.h"
#include "yaml-cpp/node/detail/impl.h"
#include "yaml-cpp/node/parse.h"

using namespace std;

bool gReadGps = true;

void signal_callback_handler(int signum) {
	printf("Caught signal %d\n", signum);
	gReadGps = false;
}

int main(int argc, char** argv) {
	if(argc < 2)
	{
		cout << "usage: ./gpsd_example *.yaml" << endl;
		exit(0);
	}

	signal(SIGINT, signal_callback_handler);
	system("killall gpsd");
	usleep(100000);

	char *config_path = argv[1];
	RtkReading rtkRead(config_path);
	rtk_data_t rtk_data;

	while (gReadGps) {
		rtk_data = rtkRead.get_rtk_data();
		
		cout << std::setprecision(12);
		cout << rtk_data.local_time << " " 
			 << rtk_data.gps_time << " "
			 << rtk_data.latitude << " "
             << rtk_data.longitude << " "
             << rtk_data.altitude << " "
			 << rtk_data.utmx << " " 
			 << rtk_data.utmy << " "
			 << rtk_data.pos_status << " "
			 << rtk_data.reference_heading << " "
			 << rtk_data.attitude_heading << " "
			 << rtk_data.gps_mode << " "
			 << std::endl;
			 
		usleep(200000);
	}
	return 0;
}
