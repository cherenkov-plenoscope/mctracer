
#=======================================================================
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "-std=gnu++11 -Wall -pedantic -O3") # 
#=======================================================================
project( tracer )
#=======================================================================
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package( OpenCV REQUIRED )
add_executable( 
tracer tracer.cpp 
TracerException.cpp
CsvHandler.cpp
#ConfigFileHandle.cpp 
WorldFactory.cpp
SensorIntersection.cpp 
ReflectionProperties.cpp 
ColourProperties.cpp 
GlobalSettings.cpp 
Vector3D.cpp 
Rotation3D.cpp 
HomoTrafo3D.cpp 
Intersection.cpp 
CartesianFrame.cpp 
OctTreeCube.cpp
SurfaceEntity.cpp 
Ray.cpp 
Photon.cpp
CameraRay.cpp
ListOfPropagations.cpp
ListOfInteractions.cpp
CameraDevice.cpp 
CameraImage.cpp
PinHoleCamera.cpp 
ApertureCamera.cpp 
LightSource.cpp 
FreeOrbitCamera.cpp 
Triangle.cpp
Plane.cpp 
Disc.cpp 
SensorDisc.cpp 
Sphere.cpp
Cylinder.cpp
OpticalMirror.cpp
OpticalMirrorRound.cpp
OpticalMirrorHexagonal.cpp
FactTelescope.cpp
OpticalMirrorSphericHexagonal.cpp
OpticalMirrorParabolicRound.cpp
OpticalMirrorSphericRound.cpp
OpticalMirrorParabolicHexagonal.cpp
OpticalMirrorEllipsoidHexagonal.cpp
pugixml.cpp
)
include_directories(/usr/include)
include_directories(/usr/local/include)
target_link_libraries( tracer ${OpenCV_LIBS} )
#=======================================================================
#set(Boost_USE_STATIC_LIBS OFF) 
#set(Boost_USE_MULTITHREADED ON)  
#set(Boost_USE_STATIC_RUNTIME OFF) 
##BOOST LOG
#ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)
#FIND_PACKAGE( Boost COMPONENTS log REQUIRED )
#include_directories( ${Boost_INCLUDE_DIR} )
#TARGET_LINK_LIBRARIES(tracer ${Boost_LOG_LIBRARY} )
##BOOST SPIRIT
#FIND_PACKAGE( Boost 1.56 REQUIRED )
#=======================================================================
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
#=======================================================================
# GTest
################################
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()

################################
# Unit Tests
################################
# Add test cpp file
add_executable( tracerUnitTests 
./test/test.cpp 
Vector3D.cpp
Rotation3D.cpp
HomoTrafo3D.cpp
GlobalSettings.cpp 
CsvHandler.cpp
ColourProperties.cpp 
TracerException.cpp
ReflectionProperties.cpp 
SurfaceEntity.cpp
Plane.cpp
CartesianFrame.cpp
OctTreeCube.cpp
Intersection.cpp
Ray.cpp
Photon.cpp
FreeOrbitCamera.cpp
CameraDevice.cpp
CameraImage.cpp
PinHoleCamera.cpp
ApertureCamera.cpp
CameraRay.cpp
ListOfInteractions.cpp
WorldFactory.cpp
pugixml.cpp
FactTelescope.cpp
Triangle.cpp
Disc.cpp 
Cylinder.cpp
Sphere.cpp
OpticalMirrorHexagonal.cpp
OpticalMirrorSphericHexagonal.cpp
OpticalMirrorRound.cpp
OpticalMirrorHexagonal.cpp
OpticalMirror.cpp
)
# Link test executable against gtest & gtest_main
target_link_libraries(tracerUnitTests gtest gtest_main)
target_link_libraries( tracerUnitTests ${OpenCV_LIBS} )
add_test( tracerUnitTests tracerUnitTests )


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

