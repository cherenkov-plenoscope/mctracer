# mctracer scientific raytracing
# Author Sebastian Mueller
#-------------------------------------------------------------------------------
cmake_minimum_required(VERSION 2.8)
set (CMAKE_CXX_FLAGS "-std=gnu++11 -Wall -pedantic -O3 -Idir")
#-------------------------------------------------------------------------------
project( mctracer )
#-------------------------------------------------------------------------------
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)
find_package( OpenCV REQUIRED )

set(SOURCE
	${CMAKE_CURRENT_SOURCE_DIR}/Vector3D.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Rotation3D.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/HomoTrafo3D.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Intersection.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Frame.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/SurfaceEntity.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Ray.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/RayForPropagation.cpp 	
	${CMAKE_CURRENT_SOURCE_DIR}/DistanceMeter.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RayAndBoundingSphereInteraction.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RayAndFrameInteraction.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Photon.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/FresnelRefractionAndReflection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ListOfPropagations.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/PropagationEnvironment.cpp	
	${CMAKE_CURRENT_SOURCE_DIR}/TrajectoryFactory.cpp	

	${CMAKE_CURRENT_SOURCE_DIR}/QuadraticEquation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/XyPlaneRayIntersectionEquation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ZaxisCylinderRayIntersectionEquation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SphericalCapRayIntersectionEquation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/RectangularPrismZ.cpp	
	${CMAKE_CURRENT_SOURCE_DIR}/CylinderPrismZ.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/HexagonalPrismZ.cpp	
	${CMAKE_CURRENT_SOURCE_DIR}/ReflectionProperties.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/AbsorptionProperties.cpp	
	${CMAKE_CURRENT_SOURCE_DIR}/RefractiveIndex.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Color.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TracerException.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/GlobalSettings.cpp 

	${CMAKE_CURRENT_SOURCE_DIR}/WorldFactory.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Function1D.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/Triangle.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Plane.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Disc.cpp 
	${CMAKE_CURRENT_SOURCE_DIR}/Sphere.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/Cylinder.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SurfaceWithOuterPrismBound.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SphereCapWithCylinderBound.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SphereCapWithHexagonalBound.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/TelescopeFrame.cpp

	${CMAKE_CURRENT_SOURCE_DIR}/HexGridXy.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SegmetedReflectorGenerator.cpp
	
	${CMAKE_CURRENT_SOURCE_DIR}/BiConvexLens.cpp

# python extension
	${CMAKE_CURRENT_SOURCE_DIR}/mcPYtracer/testPyTracer.cpp
)
add_subdirectory("${PROJECT_SOURCE_DIR}/Tools")
add_subdirectory("${PROJECT_SOURCE_DIR}/XmlIO")
add_subdirectory("${PROJECT_SOURCE_DIR}/MmcsCorsikaFileIO")
add_subdirectory("${PROJECT_SOURCE_DIR}/Cameras")

add_executable( mctracer mctracer.cpp ${SOURCE} )

include_directories(/usr/include)
include_directories(/usr/local/include)
target_link_libraries( mctracer ${OpenCV_LIBS} )
#-------------------------------------------------------------------------------
find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()
#-------------------------------------------------------------------------------
# GTest

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable( mctracerTests ./test/test.cpp ${SOURCE})

target_link_libraries( mctracerTests gtest gtest_main)
target_link_libraries( mctracerTests ${OpenCV_LIBS} )
add_test( mctracerTests mctracerTests )
#-------------------------------------------------------------------------------
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
#-------------------------------------------------------------------------------
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