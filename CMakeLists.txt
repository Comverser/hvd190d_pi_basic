cmake_minimum_required (VERSION 3.5)
project(hvd190d_pi_basic)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

include_directories(src/apps)
include_directories(src/libs)
file(GLOB SOURCES_APPS "src/apps/*.cpp")
file(GLOB SOURCES_LIBS "src/libs/*.cpp")

add_executable(demo ${SOURCES_APPS} ${SOURCES_LIBS})

find_library(wiringPi_LIB wiringPi)
target_link_libraries(demo ${wiringPi_LIB})


#include_directories(${PROJECT_SOURCE_DIR}/src/apps)
#include_directories(${PROJECT_SOURCE_DIR}/src/libs)
#set (source_dir_apps "${PROJECT_SOURCE_DIR}/src/apps/")
# Wildcard additions
#file (GLOB source_files_apps "${source_dir_apps}/*.cpp")
#set (source_dir_libs "${PROJECT_SOURCE_DIR}/src/libs/")
# Wildcard additions
#file (GLOB source_files_apps "${source_dir_libs}/*.cpp")
#add_executable (Demo ${source_files_apps} ${source_files_libs})

