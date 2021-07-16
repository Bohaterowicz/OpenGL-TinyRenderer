cmake_minimum_required(VERSION 3.0.0)
project(TinyRenderer VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_definitions(GLEW_STATIC)

INCLUDE_DIRECTORIES(./src ./src/win32 ./externals/GLEW/include ./externals/GLM ./glimg)

add_compile_options(/W2 /WX)
add_compile_definitions(DEBUG MSVC)

file(GLOB GLIMG_SOURCES "./glimg/*.cpp")
file(GLOB SOURCES "src/*.cpp")
file(GLOB WIN32_SOURCES "src/win32/*.cpp")

add_library(TinyRenderer ${SOURCES})

add_library(glimg ${GLIMG_SOURCES})

find_library(GLEW_LIB glew32s HINTS externals/GLEW/lib)

find_package(OpenGL REQUIRED)

TARGET_LINK_LIBRARIES(TinyRenderer ${GLEW_LIB} ${GLFW_LIB} glimg opengl32)

add_executable(win32_TinyRenderer WIN32 ${WIN32_SOURCES})
target_link_libraries(win32_TinyRenderer TinyRenderer user32.lib gdi32.lib winmm.lib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
