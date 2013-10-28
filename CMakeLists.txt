#=======================================================================
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "-std=gnu++11 -O3") # -Wall
#=======================================================================
project( tracer )
#=======================================================================
find_package( OpenCV REQUIRED )
add_executable( 
tracer tracer.cpp 
ConfigFileHandle.cpp 
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
PinHoleCamera.cpp 
ApertureCamera.cpp 
LightSource.cpp 
FreeOrbitCamera.cpp 
Plane.cpp 
Disk.cpp 
SensorDisk.cpp 
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
)
include_directories(/usr/include)
target_link_libraries( tracer ${OpenCV_LIBS} )
#=======================================================================
#BOOST
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON) 
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost 1.46.1 COMPONENTS regex)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    #add_executable( tracer tracer.cpp)
    target_link_libraries(tracer ${Boost_LIBRARIES}    )                                                                                                                                                                                                                        
endif()
#=======================================================================
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
#=======================================================================
# configiration file library: libconfig
target_link_libraries(tracer config++)
