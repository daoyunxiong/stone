CC = g++
CFLAGS =-std=c++14 -fpermissive
C_P_FLAG = -std=c++11
LIBS=-lsbp -lserialport -lpthread 
all:
	$(CC) Kalman.cpp utm.c  rtk_gps.cpp main.cpp -o getgps  $(CFLAGS) $(LIBS)
clean:
	rm getgps
