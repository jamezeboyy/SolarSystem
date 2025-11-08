

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(ARCHITECTURE x64)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(ARCHITECTURE x86)
else()
    message(FATAL_ERROR "Unknown target architecture")
endif()

if(WIN32)
    if (MSVC)
        message(STATUS "Building with MSVC toolchain")
    else()
        message(FATAL_ERROR "This project requires MSVC to match precompiled libraries")
    endif()
    include(${PROJECT_SOURCE_DIR}/build/windows.cmake)
else()
    include(${PROJECT_SOURCE_DIR}/build/linux.cmake)
endif()


find_package(OpenGL REQUIRED)


message(STATUS "Found glfw: ${GLFW_LIBRARY}")


add_subdirectory(${PROJECT_SOURCE_DIR}/source)