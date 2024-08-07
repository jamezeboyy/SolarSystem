cmake_minimum_required(VERSION 3.16)
project(SolarSystem)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

if(WIN32)
    execute_process(
            COMMAND powershell -Command "(Get-ItemProperty -Path 'HKLM:\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion' -Name 'ReleaseId').ReleaseId"
            OUTPUT_VARIABLE WINDOW_BUILD_VER
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
else()
    message(FATAL_ERROR "This script is intended for Windows only.")
endif()

if(NOT WINDOW_BUILD_VER)
    message(FATAL_ERROR "Failed to retrieve Windows build version.")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE x64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCHITECTURE x86)
else()
    message("Unknown target architecture")
endif()



set(OPENGL_LIBRARY C:/Program Files \(x86\)/Windows Kits/10/Lib/${WINDOW_BUILD_VER}/um/x64/OpenGL32.Lib)
set(OPENGL_DLL C:/Windows/System32/opengl32.dll)
set(OPENGL_INCLUDE_DIR "C:/Program Files (x86)/Windows Kits/10/Include/${WINDOW_BUILD_VER}/um")

include_directories(${PROJECT_SOURCE_DIR}/include/${ARCHITECTURE})

include_directories(${OPENGL_INCLUDE_DIR} include/common)

link_directories(${PROJECT_SOURCE_DIR}/lib/${ARCHITECTURE})

find_package(OpenGL REQUIRED)

set(SOURCE_DIR source)


set(SOURCES
    ${SOURCE_DIR}/glad.c
    ${SOURCE_DIR}/main.cpp
)

add_executable(SolarSystem ${SOURCES})

target_link_libraries(SolarSystem glfw3 ${OPENGL_LIBRARY} opengl32)
