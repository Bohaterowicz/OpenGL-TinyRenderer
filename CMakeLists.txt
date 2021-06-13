cmake_minimum_required(VERSION 3.0.0)
project(TinyRenderer VERSION 0.1.0)

add_compile_definitions(GLEW_STATIC)

INCLUDE_DIRECTORIES(./src ./src/win32 ./externals/GLEW/include ./externals/GLFW/include ./externals/GLM)

add_compile_options(/W2 /WX)
add_compile_definitions(DEBUG)

file(GLOB SOURCES "src/*.cpp")
file(GLOB WIN32_SOURCES "src/win32/*.cpp")

add_library(TinyRenderer ${SOURCES})

find_library(GLEW_LIB glew32s HINTS externals/GLEW/lib)

find_package(OpenGL REQUIRED)

TARGET_LINK_LIBRARIES(TinyRenderer ${GLEW_LIB} ${GLFW_LIB} opengl32)

add_executable(win32_TinyRenderer WIN32 ${WIN32_SOURCES})
target_link_libraries(win32_TinyRenderer TinyRenderer user32.lib gdi32.lib winmm.lib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
