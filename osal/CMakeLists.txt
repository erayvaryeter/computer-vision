set(project_name osal)
cmake_minimum_required (VERSION 3.15)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

project(${project_name} LANGUAGES C CXX ASM)

include(conan)

set(CMAKE_VERBOSE_MAKEFILE TRUE)

conan_check(VERSION 1.15.0 REQUIRED)
conan_cmake_run(CONANFILE conanfile.py BASIC_SETUP CMAKE_TARGETS KEEP_RPATHS UPDATE)

# Include sub-projects.
add_subdirectory("osal-core")
add_subdirectory("osal-windows")