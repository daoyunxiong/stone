IDIR =-I./ -I/home/dyxiong/workstation/adas/boost_1_58_0/
CC = g++
CFLAGS = $(IDIR)  -std=c++14 -fpermissive
C_FLAG = -L/usr/local/lib -lopencv_stitching -lopencv_superres -lopencv_videostab -lopencv_photo -lopencv_aruco -lopencv_bgsegm -lopencv_bioinspired -lopencv_ccalib -lopencv_dpm -lopencv_face -lopencv_freetype -lopencv_fuzzy -lopencv_line_descriptor -lopencv_optflow -lopencv_reg -lopencv_rgbd -lopencv_saliency -lopencv_sfm -lopencv_stereo -lopencv_structured_light -lopencv_phase_unwrapping -lopencv_surface_matching -lopencv_tracking -lopencv_datasets -lopencv_text -lopencv_plot -lopencv_xfeatures2d -lopencv_shape -lopencv_video -lopencv_ximgproc -lopencv_calib3d -lopencv_features2d -lopencv_flann -lopencv_xobjdetect -lopencv_objdetect -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_ml -lopencv_xphoto -lopencv_imgproc -lopencv_core
C_P_FLAG = -std=c++11

LIBS=-lsbp -lserialport -lpthread 

all:
	$(CC) Kalman.cpp utm.c  rtk_gps.cpp main.cpp -o getgps  $(CFLAGS) $(C_FLAG) $(LIBS)
clean:
	rm getgps
