IDIR =./
CC = g++
CFLAGS=-I$(IDIR)  -std=c++11 -fpermissive
LIBS=-lsbp -lserialport -lpthread

all:
	$(CC) gnss.c MadgwickAHRS.cpp rtk_gps.cpp main.cpp -o getgps  $(CFLAGS) $(LIBS)
clean:
	rm getgps
