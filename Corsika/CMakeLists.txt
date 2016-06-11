add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/EventIo")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/MagicMonteCarloSimulation")
set(SOURCE
   	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/Tools.cpp
   	PARENT_SCOPE
)