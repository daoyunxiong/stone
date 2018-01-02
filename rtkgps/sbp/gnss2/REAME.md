1. install libsbp library

libsbp provides C bindings for Swift Binary Protocol (SBP)
the native binary protocol used by the Piksi GPS receiver. It is intended to be as portable as possible and is written in standards compliant C with no dependencies other than the standard C libraries.

$git clone https://github.com/swift-nav/libsbp.git
$sudo apt-get install build-essential pkg-config cmake doxygen check

$cd c/
$mkdir build
$cd build

$cmake ../
$make
$sudo make install
$sudo ldconfig

2. install libserialport

An example C program, intended to be run on a host computer connected to the Piksi, that parses incoming SBP messages from the Piksi and prints some of them to the screen.

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

$ ./libsbp_example
usage: ./libsbp_example [-p serial port]

https://github.com/swift-nav/libsbp
https://github.com/swift-nav/libsbp/tree/ca5743b138fb44560d83057f9c49d79c88593904/c
https://github.com/swift-nav/libsbp/tree/ca5743b138fb44560d83057f9c49d79c88593904/c/example

3. how to use it 

main.cpp is an example to use the interface to get the gps data (including imu, magneti,lat,lon)


