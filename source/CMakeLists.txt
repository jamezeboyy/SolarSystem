
set(SOURCES main.cpp glad.c)

add_executable(SolarSystem ${SOURCES})

include_directories(${PROJECT_SOURCE_DIR}/include/common)
include_directories(${OPENGL_INCLUDE_DIR})

target_link_libraries(SolarSystem ${GLFW_LIBRARY} OpenGL)