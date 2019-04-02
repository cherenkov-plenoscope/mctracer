add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/camera_operator")
set(SOURCE
    ${SOURCE}
    ${CMAKE_CURRENT_SOURCE_DIR}/Time.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FlyingCamera.cpp
    PARENT_SCOPE
)