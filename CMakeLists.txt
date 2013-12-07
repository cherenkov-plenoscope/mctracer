#=======================================================================
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "-std=gnu++11 -Wall -O3") # 
#=======================================================================
project( tracer )
#=======================================================================
find_package( OpenCV REQUIRED )
add_executable( 
tracer tracer.cpp 
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
SurfaceEntity.cpp 
Ray.cpp 
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
