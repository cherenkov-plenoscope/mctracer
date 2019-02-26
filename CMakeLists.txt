# Copyright 2011-2018 Sebastian A. Mueller
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "-std=c++1z -Wall -Wextra -pedantic -O1 -g -I")
project(mctracer)

#--------------------------------
# Assert compiler supports C++ 11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 5)
    message(FATAL_ERROR "Insufficient gcc version, need at least gcc 5")
  endif()
endif()
#-------------------
# External libraries

# 	OpenCV
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})
#-------------------------------------------------------------------------------
# Cmake
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
#-----------
# The Source
set(SOURCE
	${CMAKE_CURRENT_SOURCE_DIR}/json.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/eventio.cpp)
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict")
add_subdirectory("${PROJECT_SOURCE_DIR}/corsika")
add_subdirectory("${PROJECT_SOURCE_DIR}/plenoscope")
add_subdirectory("${PROJECT_SOURCE_DIR}/try_to_read")
add_subdirectory("${PROJECT_SOURCE_DIR}/Tools")
add_subdirectory("${PROJECT_SOURCE_DIR}/signal_processing")
add_subdirectory("${PROJECT_SOURCE_DIR}/docopt")
add_subdirectory("${PROJECT_SOURCE_DIR}/viewer")
#-------------------------------
# mctracer library
add_library(mct SHARED ${SOURCE})
include_directories(/usr/include)
include_directories(/usr/local/include)
target_link_libraries(mct ${OpenCV_LIBS})

#---------
# Show exe
add_executable(mctShow viewer/Show.cpp)
target_link_libraries(mctShow mct)
target_link_libraries(mctShow docopt)

#---------
# CameraServer exe
add_executable(mctCameraServer main/CameraServer.cpp)
target_link_libraries(mctCameraServer mct)
target_link_libraries(mctCameraServer docopt)

#---------------
# Propagate exe
add_executable(mctPropagate main/Propagate.cpp)
target_link_libraries(mctPropagate mct)
target_link_libraries(mctPropagate docopt)

#----------------
# ShowPhotons exe
add_executable(mctShowPhotons viewer/ShowPhotons.cpp)
target_link_libraries(mctShowPhotons mct)
target_link_libraries(mctShowPhotons docopt)

#------------------------
# light field calibration
add_executable(mctPlenoscopeCalibration main/PlenoscopeCalibration.cpp)
target_link_libraries(mctPlenoscopeCalibration mct)
target_link_libraries(mctPlenoscopeCalibration stdc++fs)
target_link_libraries(mctPlenoscopeCalibration docopt)

#------------------------
# EventIOConverter
add_executable(mctEventIOConverter main/EventIOConverter.cpp)
target_link_libraries(mctEventIOConverter mct)
target_link_libraries(mctEventIOConverter stdc++fs)
target_link_libraries(mctEventIOConverter docopt)

#------------------------
# light field propagation
add_executable(mctPlenoscopePropagation main/PlenoscopePropagation.cpp)
target_link_libraries(mctPlenoscopePropagation mct)
target_link_libraries(mctPlenoscopePropagation stdc++fs)
target_link_libraries(mctPlenoscopePropagation docopt)

#------------------------
# light field raw photon propagation
add_executable(mctPlenoscopeRawPhotonPropagation main/PlenoscopeRawPhotonPropagation.cpp)
target_link_libraries(mctPlenoscopeRawPhotonPropagation mct)
target_link_libraries(mctPlenoscopeRawPhotonPropagation stdc++fs)
target_link_libraries(mctPlenoscopeRawPhotonPropagation docopt)

#---------
# STL scaling
add_executable(mctScaleStl main/ScaleStl.cpp)
target_link_libraries(mctScaleStl mct)
target_link_libraries(mctScaleStl docopt)

#------------------------
# FACT propagation
add_executable(mctFactPropagation ./main/FactPropagation.cpp)
target_link_libraries(mctFactPropagation mct)
target_link_libraries(mctFactPropagation stdc++fs)
target_link_libraries(mctFactPropagation docopt)

#---------------------------------
# unit test exe
add_subdirectory("${PROJECT_SOURCE_DIR}/tests")
add_executable(mctTest ./tests/test.cpp ${TEST_SOURCE})
target_link_libraries(mctTest mct)
