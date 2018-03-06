IDIR =-I./ -I/home/dyxiong/workstation/adas/boost_1_58_0/
CC = g++
CFLAGS = $(IDIR)  -std=c++14 -fpermissive
LIBS=-lsbp -lserialport -lpthread 

getgps:
	$(CC) Kalman.cpp MahonyAHRS.cpp MadgwickAHRS.cpp utm.c rtk_gps.cpp main.cpp -o $@  $(CFLAGS) $(LIBS)
clean:
	rm getgps
