IDIR =./
CC = g++
CFLAGS=-I$(IDIR)  -std=c++11 -fpermissive
LIBS=-lsbp -lserialport -lpthread

getgps:
	$(CC) gnss.c rtk_gps.cpp main.cpp -o $@  $(CFLAGS) $(LIBS)
clean:
	rm getgps
