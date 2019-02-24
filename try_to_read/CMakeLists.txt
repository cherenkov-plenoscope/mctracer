add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/PhotonsReaderWrapper")
set(SOURCE
   	${SOURCE}
	${CMAKE_CURRENT_SOURCE_DIR}/PhotonsReader.cpp
   	PARENT_SCOPE
)