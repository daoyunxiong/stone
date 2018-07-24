INCPATH =-I ./
LIBS = -lpthread -lgps -lm -lyaml-cpp
LIBP = -L /home/dyxiong/workstation/git_proj/stone/rtkgps/gpsd_lib/gpsd-3.17
CXX = g++ 
CC = gcc 
CXXFLAGS = -std=c++11
OBJECTS = main.o utm.o RtkReading.o
TARGET = gpsd_example

.cpp.o: 
	$(CXX) -c $(CXXFLAGS) -o $@ $<
.c.o: 
	$(CC) -c -o $@ $<
$(TARGET) : $(OBJECTS) 
	$(CXX) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(INCPATH)  $(LIBS) 

clean: 
	rm *.o -rf 
	rm $(TARGET)
