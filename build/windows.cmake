message(STATUS "Loading Windows config")

execute_process(
        COMMAND powershell -Command "(Get-ItemProperty -Path 'HKLM:\\SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion' -Name 'ReleaseId').ReleaseId"
        OUTPUT_VARIABLE WINDOW_BUILD_VER
        OUTPUT_STRIP_TRAILING_WHITESPACE
)


if(NOT WINDOW_BUILD_VER)
    message(FATAL_ERROR "Failed to retrieve Windows build version.")
endif()


set(OPENGL_LIBRARY C:/Program Files \(x86\)/Windows Kits/10/Lib/${WINDOW_BUILD_VER}/um/x64/OpenGL32.Lib)
set(OPENGL_DLL C:/Windows/System32/opengl32.dll)
set(OPENGL_INCLUDE_DIR "C:/Program Files (x86)/Windows Kits/10/Include/${WINDOW_BUILD_VER}/um")


set(GLFW_LIBRARY "${PROJECT_SOURCE_DIR}/lib/${ARCHITECTURE}/glfw3.dll")

include_directories(${PROJECT_SOURCE_DIR}/include/${ARCHITECTURE})

link_directories(${PROJECT_SOURCE_DIR}/lib/${ARCHITECTURE})
