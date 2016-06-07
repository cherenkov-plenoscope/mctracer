add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Calibration")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/LightFieldSensor")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/NightSkyBackground")
set(SOURCE
   	${SOURCE}
  	${CMAKE_CURRENT_SOURCE_DIR}/EventFormats.cpp
   	PARENT_SCOPE
)