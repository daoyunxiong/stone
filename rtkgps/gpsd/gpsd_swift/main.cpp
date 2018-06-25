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

	YAML::Node global_config = YAML::LoadFile(argv[1]);
	int rate = global_config["debug"]["antenna_num"].as<int>();
	cout << rate << endl;
	cout << global_config["debug"]["port_pos"].as<string>()<< endl;

//	char port_pos[] = "/dev/ttyACM0";
    std::string port_pos_s = global_config["debug"]["port_pos"].as<std::string>();
	char *port_pos = (char*) port_pos_s.c_str();
	cout << port_pos << endl;

	char port_heading[] = "/dev/ttyUSB0";
	RtkReading rtkRead(port_pos, port_pos);
	rtk_data_t rtk_data;

	while (gReadGps) {
		rtk_data = rtkRead.get_rtk_data();
		cout << std::setprecision(12);
		cout << rtk_data.local_time << " " << rtk_data.llh_t.lat << " "
             << rtk_data.llh_t.log << " "
			 << rtk_data.utm.x << " " 
			 << rtk_data.utm.y << " "
			 << rtk_data.dual_antenna_heading << " "
	 		 << rtk_data.status << endl;
		usleep(200000);
	}
	return 0;
}
