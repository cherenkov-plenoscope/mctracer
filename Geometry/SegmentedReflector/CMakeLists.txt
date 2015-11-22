set(SOURCE
   	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/SegmentedReflectorConfig.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SegmentedReflectorGeometry.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/SegmentedReflectorFactory.cpp
   	PARENT_SCOPE
)

set(HEADERS
   	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/SegmentedReflector.h
   	PARENT_SCOPE
)