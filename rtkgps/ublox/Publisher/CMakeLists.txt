cmake_minimum_required(VERSION 2.8.3)
project(tjhPublisher)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -std=c++11")

## Include headers
include_directories(include)

# header library
add_library(${PROJECT_NAME} INTERFACE)

target_compile_definitions(${PROJECT_NAME}
    INTERFACE LIBRARY_HEADER_ONLY
)
target_include_directories(${PROJECT_NAME} INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/tjhPublisher>
    )
target_sources(${PROJECT_NAME} INTERFACE 
  ${CMAKE_CURRENT_SOURCE_DIR}/include/tjhPublisher/Publisher.h
)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/tjhPublisher/Publisher.h
  DESTINATION include/tjhPublisher
)