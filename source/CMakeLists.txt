set(SOLAR_TARGET SolarSystem)
set(SOURCES main.cpp glad.c)

include_directories(${PROJECT_SOURCE_DIR}/include/common)
include_directories(${OPENGL_INCLUDE_DIR})
if (WIN32)
    include_directories(${PROJECT_SOURCE_DIR}/include/${ARCHITECTURE})
    link_directories(${PROJECT_SOURCE_DIR}/lib/${ARCHITECTURE})
endif()

add_executable(${SOLAR_TARGET} ${SOURCES})


if (WIN32)
    target_link_libraries(${SOLAR_TARGET} glfw3 ${OPENGL_LIBRARY} opengl32)
else ()
    target_link_libraries(${SOLAR_TARGET} ${GLFW_LIBRARY} OpenGL)
endif ()