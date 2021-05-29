cmake_minimum_required(VERSION 3.0.0)
project(OpenGL_TinyRenderer VERSION 0.1.0)

add_compile_definitions(GLEW_STATIC)

INCLUDE_DIRECTORIES(.src/ ./externals/GLEW/include ./externals/GLFW/include ./externals/GLM)

file(GLOB SOURCES "src/*.cpp")

add_executable(OpenGL_TinyRenderer ${SOURCES})

find_library(GLEW_LIB glew32s HINTS externals/GLEW/lib)
find_library(GLFW_LIB glfw3 HINTS externals/GLFW/lib)

find_package(OpenGL REQUIRED)

TARGET_LINK_LIBRARIES(OpenGL_TinyRenderer ${GLEW_LIB} ${GLFW_LIB} opengl32)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
