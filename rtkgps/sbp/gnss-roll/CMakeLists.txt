cmake_minimum_required(VERSION 3.0)
project(gnss)
set(LIB_NAME "gnss")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

include_directories(/usr/local/include)

set(EXT_LIBS
        sbp
        serialport
        pthread
        )

include_directories(
        ./
        )

set(SOURCE_FILES
        rtk_gps.cpp
        utm.c
        "rtk_gps.cpp"
        )

add_library(${LIB_NAME} STATIC ${SOURCE_FILES})
target_link_libraries(${LIB_NAME} ${EXT_LIBS})
target_include_directories(${LIB_NAME} INTERFACE ./)
