# mctracer scientific ray tracing
# Author: Sebastian Mueller
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -pedantic -O2 -g -I")
#-------------------------------------------------------------------------------
project(mctracer)
#-------------------------------------------------------------------------------
# Assert compiler supports C++ 11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()
#-------------------------------------------------------------------------------
# External libraries
# OpenMP
find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

# OpenCV
find_package(OpenCV 2.4.8 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIB_DIR})
#-------------------------------------------------------------------------------
# Cmake
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH 
	${CMAKE_MODULE_PATH} 
	${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules
)
#-------------------------------------------------------------------------------
# The Source
set(SOURCE 	
	${CMAKE_CURRENT_SOURCE_DIR}/DistanceMeter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TrajectoryFactory.cpp	

	${CMAKE_CURRENT_SOURCE_DIR}/QuadraticEquation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/XyPlaneRayIntersectionEquation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ZaxisCylinderRayIntersectionEquation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SphericalCapRayIntersectionEquation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RectangularPrismZ.cpp	
	${CMAKE_CURRENT_SOURCE_DIR}/CylinderPrismZ.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/HexagonalPrismZ.cpp	
	${CMAKE_CURRENT_SOURCE_DIR}/TracerException.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/KeyValueMap.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ToDoScheduler.cpp		

	${CMAKE_CURRENT_SOURCE_DIR}/TelescopeArrayControl.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/Triangle.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Plane.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Disc.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/LightSourceFromConfig.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Cylinder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SurfaceWithOuterPrismBound.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SphereCapWithCylinderBound.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SphereCapWithHexagonalBound.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EllipticalCapWithHexagonalBound.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/EllipticalCapRayIntersectionEquation.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/HexGridXy.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SegmetedReflectorGenerator.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/LensMaker.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/BiConvexLens.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/BiConvexLensHexBound.cpp
)
add_subdirectory("${PROJECT_SOURCE_DIR}/XmlFactory")
add_subdirectory("${PROJECT_SOURCE_DIR}/PhotonSensor")
add_subdirectory("${PROJECT_SOURCE_DIR}/Tools")
add_subdirectory("${PROJECT_SOURCE_DIR}/XmlIO")
add_subdirectory("${PROJECT_SOURCE_DIR}/MmcsCorsikaFileIO")
add_subdirectory("${PROJECT_SOURCE_DIR}/Cameras")
add_subdirectory("${PROJECT_SOURCE_DIR}/Core")
add_subdirectory("${PROJECT_SOURCE_DIR}/Geometry")
#-------------------------------------------------------------------------------
# mctracer shared linked library

add_library(mct SHARED ${SOURCE})
include_directories(/usr/include)
include_directories(/usr/local/include)
target_link_libraries(mct ${OpenCV_LIBS})
#-------------------------------------------------------------------------------
# mctracer executable

add_executable(mctracer mctracer.cpp)
target_link_libraries(mctracer mct)
#-------------------------------------------------------------------------------
# GTest unit and integration tests
include_directories("${PROJECT_SOURCE_DIR}/gtest-1.7.0/include")

add_executable(mctracerTests ./test/test.cpp)
target_link_libraries(mctracerTests gtest gtest_main)
target_link_libraries(mctracerTests mct)
#-------------------------------------------------------------------------------
# GTest executable examples

add_executable(mctracerExamples ./Docu/examples/examples.cpp)
target_link_libraries(mctracerExamples gtest gtest_main)
target_link_libraries(mctracerExamples mct)
#-------------------------------------------------------------------------------
# http://stackoverflow.com/questions/992068/building-a-python-shared-object-binding-with-cmake-which-depends-upon-external
# http://www.swig.org/Doc1.3/Introduction.html#Introduction_build_system
# This is a CMake example for Python

#FIND_PACKAGE(SWIG REQUIRED)
#INCLUDE(${SWIG_USE_FILE})

#FIND_PACKAGE(PythonLibs)
#INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Core)
#INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/Cameras)


#SET(CMAKE_SWIG_FLAGS "")

#SET_SOURCE_FILES_PROPERTIES("py_mct/mctracer.i" PROPERTIES CPLUSPLUS ON)
#SET_SOURCE_FILES_PROPERTIES("py_mct/mctracer.i" PROPERTIES SWIG_FLAGS "")

#SWIG_ADD_MODULE(mctracer python "py_mct/mctracer.i")

#SWIG_LINK_LIBRARIES(mctracer ${PYTHON_LIBRARIES})
#SWIG_LINK_LIBRARIES(mctracer mct)
