cmake_minimum_required(VERSION 3.26)
project(SolarSystem C)

SET(CMAKE_BINARY_DIR build/generators)

include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)

set(CMAKE_C_STANDARD 11)

add_executable(SolarSystem source/main.c)

# Find the opengl/system package
find_package(opengl_system REQUIRED)

# Link the opengl/system library
target_link_libraries(SolarSystem opengl::opengl)
