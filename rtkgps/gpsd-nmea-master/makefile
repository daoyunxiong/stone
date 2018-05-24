# Author: Sven Kreiss <sk@svenkreiss.com>, Mar 29, 2011

SRC  = src/main.c
SRC += src/nmeaparser/nmea_parse.c
SRC += src/nmeaparser/gpsutils.c
SRC += src/nmeaparser/geoid.c

INCDIR = ./src/nmeaparser

all:main

main:$(SRC)
	gcc -I$(INCDIR) $(SRC) -lm -lbsd -std=gnu99 -o main

clean:
	rm main
	
	
	
	
