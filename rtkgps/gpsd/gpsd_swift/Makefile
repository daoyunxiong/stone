INCPATH =./ 
LIBS = -lpthread -lgps -lm -lyaml-cpp
CXX = g++ 
CXXFLAGS = -std=c++11
OBJECTS = main.o utm.o RtkReading.o
TARGET = gpsd_example

.SUFFIXES: .o .cpp 
.cpp.o: 
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(TARGET) : $(OBJECTS) 
	$(CXX) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(LIBPATH) $(LIBS)

clean: 
	rm *.o -rf 
	rm $(TARGET)
