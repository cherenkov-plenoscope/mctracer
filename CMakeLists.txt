# Copyright 2011-2018 Sebastian A. Mueller
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "-std=c++1z -pthread -Wall -Wextra -pedantic -O1 -g -I")
project(merlict)

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

#   OpenCV
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})
#-------------------------------------------------------------------------------
# Cmake
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
#-----------
# The Source
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict")
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict_corsika")
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict_json")
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict_visual")
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict_multi_thread")
add_subdirectory("${PROJECT_SOURCE_DIR}/plenoscope")
add_subdirectory("${PROJECT_SOURCE_DIR}/signal_processing")
add_subdirectory("${PROJECT_SOURCE_DIR}/docopt")
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict_viewer")
#-------------------------------
# merlict library
add_library(mct SHARED ${SOURCE})
include_directories(/usr/include)
include_directories(/usr/local/include)
target_link_libraries(mct ${OpenCV_LIBS})

#---------
# Show exe
add_executable(mctShow merlict_viewer/Show.cpp)
target_link_libraries(mctShow mct)
target_link_libraries(mctShow docopt)

#---------
# CameraServer exe
add_executable(mctCameraServer merlict_visual/apps/CameraServer.cpp)
target_link_libraries(mctCameraServer mct)
target_link_libraries(mctCameraServer docopt)

#---------------
# Propagate exe
add_executable(mctPropagate merlict_tests/apps/Propagate.cpp)
target_link_libraries(mctPropagate mct)
target_link_libraries(mctPropagate docopt)

#----------------
# ShowPhotons exe
add_executable(mctShowPhotons merlict_viewer/ShowPhotons.cpp)
target_link_libraries(mctShowPhotons mct)
target_link_libraries(mctShowPhotons docopt)

#------------------------
# light field calibration
add_executable(mctPlenoscopeCalibration plenoscope/apps/PlenoscopeCalibration.cpp)
target_link_libraries(mctPlenoscopeCalibration mct)
target_link_libraries(mctPlenoscopeCalibration stdc++fs)
target_link_libraries(mctPlenoscopeCalibration docopt)

#------------------------
# EventIOConverter
add_executable(mctEventIOConverter merlict_corsika/apps/EventIOConverter.cpp)
target_link_libraries(mctEventIOConverter mct)
target_link_libraries(mctEventIOConverter stdc++fs)
target_link_libraries(mctEventIOConverter docopt)

#------------------------
# light field propagation
add_executable(mctPlenoscopePropagation plenoscope/apps/PlenoscopePropagation.cpp)
target_link_libraries(mctPlenoscopePropagation mct)
target_link_libraries(mctPlenoscopePropagation stdc++fs)
target_link_libraries(mctPlenoscopePropagation docopt)

#------------------------
# light field raw photon propagation
add_executable(mctPlenoscopeRawPhotonPropagation plenoscope/apps/PlenoscopeRawPhotonPropagation.cpp)
target_link_libraries(mctPlenoscopeRawPhotonPropagation mct)
target_link_libraries(mctPlenoscopeRawPhotonPropagation stdc++fs)
target_link_libraries(mctPlenoscopeRawPhotonPropagation docopt)

#------------------------
# FACT propagation
add_executable(mctFactPropagation merlict_fact/apps/FactPropagation.cpp)
target_link_libraries(mctFactPropagation mct)
target_link_libraries(mctFactPropagation stdc++fs)
target_link_libraries(mctFactPropagation docopt)

#---------------------------------
# unit test exe
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict/tests")
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict_corsika/tests")
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict_visual/tests")
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict_json/tests")
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict_multi_thread/tests")
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict_viewer/tests")
add_subdirectory("${PROJECT_SOURCE_DIR}/merlict_tests")
add_subdirectory("${PROJECT_SOURCE_DIR}/plenoscope/tests")
add_subdirectory("${PROJECT_SOURCE_DIR}/signal_processing/tests")
add_executable(
    mctTest
    merlict/tests/test.cpp
    ${TEST_SOURCE_PLENOSCOPE}
    ${TEST_SOURCE_SIGNAL_PROCESSING}
    ${TEST_SOURCE_MERLICT}
    ${TEST_SOURCE_MERLICT_CORSIKA}
    ${TEST_SOURCE_MERLICT_VISUAL}
    ${TEST_SOURCE_MERLICT_JSON}
    ${TEST_SOURCE_MERLICT_MULTI_THREAD}
    ${TEST_SOURCE_MERLICT_VIEWER}
    ${TEST_SOURCE_MERLICT_INTEGRATION})
target_link_libraries(mctTest mct)
