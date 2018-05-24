# 1.install libsbp library

libsbp provides C bindings for Swift Binary Protocol (SBP)

the native binary protocol used by the Piksi GPS receiver. It is intended to be as portable as possible and is written in standards compliant C with no dependencies other than the standard C libraries.
```
  $git clone https://github.com/swift-nav/libsbp.git
  $sudo apt-get install build-essential pkg-config cmake doxygen check
  $cd c/
  $mkdir build
  $cd build
  $cmake ../
  $make
  $sudo make install
  $sudo ldconfig
```
# 2. install libserialport 

An example C program, intended to be run on a host computer connected to the Piksi, that parses incoming SBP messages from the Piksi and prints some of them to the screen.
```
  At  first install libserialport, getting the code
  $ git clone git://sigrok.org/libserialport
  $ ./autogen.sh
  $ ./configure
  $ make
  $ sudo make install
  $sudo ldconfig
  $cd example
  $mkdir build
  $cd build
  $cmake ../
  $make
  ./libsbp_example
```
  usage: ./libsbp_example [-p serial port]

  https://github.com/swift-nav/libsbp

  https://github.com/swift-nav/libsbp/tree/ca5743b138fb44560d83057f9c49d79c88593904/c

  https://github.com/swift-nav/libsbp/tree/ca5743b138fb44560d83057f9c49d79c88593904/c/example

# 3.how to use it

  main.cpp 

    An example to use the interface to get the gps data (including imu, magneti,lat,lon)

  gnss.h gnss.c

    Get the gps data(lat,lon,imu,magnetometer) with sbp and serial port which was realized by c code.

  rtk_gps.h rtk_gps.cpp

    Function interface realized by c++ code , a class to get the gps data.

# 4.For a vehicle localization system, four coordinate systems are defined:

  Earth-Centered-Earth-Fixed (ECEF) coordinate system (E): It has an origin at the center of the Earth. The positive Z-axis goes out the Earth’s north pole; the X-axis is along the prime meridian; and the Y-axis completes the right-handed system;

  Global coordinate system (G): The North-East-Down (NED) coordinate system is defined as G with the X-axis pointing north, the Y-axis pointing east and the Z-axis pointing down to construct a right-handed coordinate system;

  Body coordinate system (B): The coordinate system of the vehicle with the X-axis pointing forwards, the Y-axis pointing left and the Z-axis pointing up;

  Sensor coordinate system (S): the three orthogonal axes of the mounted sensors. We assume that S coincides with B after sensor to body alignment calibration.
