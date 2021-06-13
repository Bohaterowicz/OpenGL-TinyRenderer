cmake_minimum_required(VERSION 3.0.0)
project(GLIMG VERSION 0.1.0)

add_compile_definitions(DEBUG_BUILD)


add_executable(GLIMG glimage.cpp glpng_loader.cpp glbmp_loader.cpp test_main.cpp win32_glimg_file.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
