# mctracer scientific ray tracing
# Author: Sebastian Achim Mueller
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "-std=c++1z -Wall -pedantic -O3 -g -I")
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
# 	OpenMP
find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# 	OpenCV
find_package(OpenCV 2.4.8 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})
#-------------------------------------------------------------------------------
# Cmake
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
#-----------
# The Source
set(SOURCE 	
	${CMAKE_CURRENT_SOURCE_DIR}/TelescopeArrayControl.cpp		
	${CMAKE_CURRENT_SOURCE_DIR}/ZdAzMount.cpp
)
add_subdirectory("${PROJECT_SOURCE_DIR}/Cameras")
add_subdirectory("${PROJECT_SOURCE_DIR}/Core")
add_subdirectory("${PROJECT_SOURCE_DIR}/Corsika")
add_subdirectory("${PROJECT_SOURCE_DIR}/Fits")
add_subdirectory("${PROJECT_SOURCE_DIR}/Scenery")
add_subdirectory("${PROJECT_SOURCE_DIR}/LensMaker")
add_subdirectory("${PROJECT_SOURCE_DIR}/Plenoscope")
add_subdirectory("${PROJECT_SOURCE_DIR}/PhotonSensor")
add_subdirectory("${PROJECT_SOURCE_DIR}/Tools")
add_subdirectory("${PROJECT_SOURCE_DIR}/Xml")
add_subdirectory("${PROJECT_SOURCE_DIR}/PhotonsReader")
add_subdirectory("${PROJECT_SOURCE_DIR}/SignalProcessing")
add_subdirectory("${PROJECT_SOURCE_DIR}/DocOpt")
#-------------------------------
# mctracer library
add_library(mct SHARED ${SOURCE})
include_directories(/usr/include)
include_directories(/usr/local/include)
target_link_libraries(mct ${OpenCV_LIBS})

#---------
# Show exe
add_executable(mctShow Main/Show.cpp)
target_link_libraries(mctShow mct)
target_link_libraries(mctShow docopt)

#---------------
# Propagate exe
add_executable(mctPropagate Main/Propagate.cpp)
target_link_libraries(mctPropagate mct)
target_link_libraries(mctPropagate docopt)

#----------------
# ShowPhotons exe
add_executable(mctShowPhotons Main/ShowPhotons.cpp)
target_link_libraries(mctShowPhotons mct)
target_link_libraries(mctShowPhotons docopt)

#------------------------
# light field calibration
add_executable(mctPlenoscopeCalibration Main/PlenoscopeCalibration.cpp)
target_link_libraries(mctPlenoscopeCalibration mct)
target_link_libraries(mctPlenoscopeCalibration stdc++fs)
target_link_libraries(mctPlenoscopeCalibration docopt)

#------------------------
# light field propagation
add_executable(mctPlenoscopePropagation Main/PlenoscopePropagation.cpp)
target_link_libraries(mctPlenoscopePropagation mct)
target_link_libraries(mctPlenoscopePropagation stdc++fs)
target_link_libraries(mctPlenoscopePropagation docopt)

#---------------------------------
# unit test exe
add_subdirectory("${PROJECT_SOURCE_DIR}/Tests/gtest-1.7.0")
include_directories("${PROJECT_SOURCE_DIR}/Tests/gtest-1.7.0/include")
add_subdirectory("${PROJECT_SOURCE_DIR}/Tests")

add_executable(mctTest ./Tests/test.cpp ${TEST_SOURCE})
target_link_libraries(mctTest gtest gtest_main)
target_link_libraries(mctTest mct)

#---------------------------------
# Benchmark
add_subdirectory("${PROJECT_SOURCE_DIR}/Benchmarks")

add_executable(mctBenchMark ./Benchmarks/benchmark.cpp ${TEST_BENCHMARK})
target_link_libraries(mctBenchMark gtest gtest_main)
target_link_libraries(mctBenchMark mct)

#---------------------------------
# HDF5
#add_executable(mctH5Test Hdf5/examples/create.cpp)
#target_link_libraries(mctH5Test mct)

#-------------------------------------------------------------------------------
# http://stackoverflow.com/questions/992068/building-a-python-shared-object-binding-with-cmake-which-depends-upon-external
# http://www.swig.org/Doc1.3/Introduction.html#Introduction_build_system
# This is a CMake example for Python

#FIND_PACKAGE(SWIG REQUIRED)
#INCLUDE(${SWIG_USE_FILE})
#
#FIND_PACKAGE(PythonLibs)
#INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})
#
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Core)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Cameras)
#
#
#SET(CMAKE_SWIG_FLAGS "")
#
#SET_SOURCE_FILES_PROPERTIES("mctracer.i" PROPERTIES CPLUSPLUS ON)
#SET_SOURCE_FILES_PROPERTIES("mctracer.i" PROPERTIES SWIG_FLAGS "")
#
#SWIG_ADD_MODULE(mctracer python "mctracer.i")
#
#SWIG_LINK_LIBRARIES(mctracer ${PYTHON_LIBRARIES})
#SWIG_LINK_LIBRARIES(mctracer mct)
#
