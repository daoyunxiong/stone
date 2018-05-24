cmake_minimum_required(VERSION 2.8.3)
project(sins)

set(CMAKE_C_FLAGS "-std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -std=c++11")
set(CMAKE_BUILD_TYPE Release)

set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
endif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

# Boost
set(Boost_LIBRARY_DIRS /home/dyxiong/workstation/adas/boost_1_58_0/stage/lib)
set(Boost_LIBRARY_DIR /home/dyxiong/workstation/adas/boost_1_58_0/stage/lib)
set(Boost_INCLUDE_DIRS /home/dyxiong/workstation/adas/boost_1_58_0/)
set(Boost_INCLUDE_DIR /home/dyxiong/workstation/adas/boost_1_58_0/)
set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
find_package(Boost REQUIRED COMPONENTS  filesystem system)
include_directories(${Boost_INCLUDE_DIRS})

# GFlags
find_package (GFlags REQUIRED)
include_directories (${GFLAGS_INCLUDE_DIR})

# Glog
set(GLOG_INCLUDE_DIR_HINTS /usr/local/Cellar/glog/0.3.4_1/include)
set(GLOG_LIBRARY_DIR_HINTS /usr/local/Cellar/glog/0.3.4_1/lib)

find_package(Glog REQUIRED)
include_directories (${GLOG_INCLUDE_DIRS})

include_directories(/usr/local/include)
link_directories(/usr/local/lib)

set(EXT_LIBS
    glog
    gflags
    ${Boost_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARY}
    -lpthread)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/build)

add_subdirectory(../Publisher tjhPubliser)
set(Publisher_INC ../Publisher/include)

include_directories(include/gnss_grid_core)

set(codegen_lib_path codegen/lib)
include_directories(codegen)
include_directories(${codegen_lib_path}/common)
include_directories(${codegen_lib_path}/Gauss_projection_Positive)
# include_directories(${codegen_lib_path}/gnss_update)
# include_directories(${codegen_lib_path}/imu_update)
include_directories(${codegen_lib_path}/gnss_sins_ekf_loose_integration)

file(GLOB codegen_lib 
    "${codegen_lib_path}/common/*.c"
    "${codegen_lib_path}/Gauss_projection_Positive/*.c"
#    "${codegen_lib_path}/gnss_update/*.c"
#    "${codegen_lib_path}/imu_update/*.c"
    "${codegen_lib_path}/gnss_sins_ekf_loose_integration/*.c")

set(gnss_src  src/gnss_grid.cpp 
               src/circle_buffer.cpp
               src/serial.cpp
               ${codegen_lib} 
               codegen/codegen_main.c)

add_library(gnss_grid_core STATIC 
               src/gnss_grid.cpp 
               src/circle_buffer.cpp
               src/serial.cpp
               ${codegen_lib} 
               codegen/codegen_main.c)

target_include_directories(gnss_grid_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(gnss_grid_core tjhPubliser ${EXT_LIBS})
include_directories(${Publisher_INC})
include_directories(./include)

add_executable(ublox_reader ${gnss_src} src/ublox_reader.cpp)
add_executable(test_gnss_grid ${gnss_src} src/test_gnss_grid.cpp)
target_link_libraries(ublox_reader ${EXT_LIBS})
target_link_libraries(test_gnss_grid ${EXT_LIBS})

