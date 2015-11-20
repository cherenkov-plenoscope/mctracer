add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/SegmentedReflector")
set(SOURCE
   	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/Sphere.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StereoLitographyIo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StereoLitographyBinaryReader.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/StereoLitographyBinaryWriter.cpp
   	PARENT_SCOPE
)

set(HEADERS
   	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/Sphere.h
	${CMAKE_CURRENT_SOURCE_DIR}/StereoLitographyIo.h
   	PARENT_SCOPE
)