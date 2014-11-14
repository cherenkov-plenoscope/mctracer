
#=======================================================================
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "-std=gnu++11 -Wall -pedantic -O3") # 
#=======================================================================
project( mctracer )
#=======================================================================
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package( OpenCV REQUIRED )
add_executable( 
mctracer mctracer.cpp 
mctracerException.cpp
XmlIoException.cpp
CsvHandler.cpp
XmlFileIo.cpp
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
FreeOrbitCamera.cpp 
Triangle.cpp
Plane.cpp 
Disc.cpp 
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
# python extension
mcPYtracer/testPyTracer.cpp
Functions.cpp
SmartImage.cpp
Function1D.cpp
)
include_directories(/usr/include)
include_directories(/usr/local/include)
target_link_libraries( mctracer ${OpenCV_LIBS} )
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
add_executable( mctracerUnitTests 
./test/test.cpp 
Vector3D.cpp
Rotation3D.cpp
HomoTrafo3D.cpp
GlobalSettings.cpp 
CsvHandler.cpp
ColourProperties.cpp 
mctracerException.cpp
XmlIoException.cpp
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
ListOfPropagations.cpp
XmlFileIo.cpp
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
Functions.cpp
mcPYtracer/testPyTracer.cpp
SmartImage.cpp
Function1D.cpp
)
# Link test executable against gtest & gtest_main
target_link_libraries(mctracerUnitTests gtest gtest_main)
target_link_libraries( mctracerUnitTests ${OpenCV_LIBS} )
add_test( mctracerUnitTests mctracerUnitTests )

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
#=======================================================================
# http://stackoverflow.com/questions/992068/building-a-python-shared-object-binding-with-cmake-which-depends-upon-external
# http://www.swig.org/Doc1.3/Introduction.html#Introduction_build_system
# This is a CMake example for Python

FIND_PACKAGE(SWIG REQUIRED)
INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

SET(CMAKE_SWIG_FLAGS "")

SET_SOURCE_FILES_PROPERTIES(../mctracer/mcPYtracer/testPyTracer.i PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(../mctracer/mcPYtracer/testPyTracer.i PROPERTIES SWIG_FLAGS "-includeall")

SWIG_ADD_MODULE(mctracer python 
../mctracer/mcPYtracer/testPyTracer.i 
../mctracer/mcPYtracer/testPyTracer.cpp
)

SWIG_LINK_LIBRARIES(mctracer ${PYTHON_LIBRARIES})
SWIG_LINK_LIBRARIES(mctracer gtest gtest_main)
SWIG_LINK_LIBRARIES(mctracer ${OpenCV_LIBS})