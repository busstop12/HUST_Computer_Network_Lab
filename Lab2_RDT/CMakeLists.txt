cmake_minimum_required(VERSION 3.10)
project(GBN)



SET(CMAKE_C_COMPTLER GCC)
set(CMAKE_CXX FLAGS -fexec-charset=GBK)
add_definitions(-std=c++11)
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
FIND_LIBRARY(NETSIM_LIB libnetsim.a ${PROJECT_SOURCE_DIR}/lib)

add_subdirectory(${PROJECT_SOURCE_DIR}/src/StopWait)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/GBN)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/SR)
add_subdirectory(${PROJECT_SOURCE_DIR}/src/TCP)