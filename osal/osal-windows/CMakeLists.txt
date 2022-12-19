cmake_minimum_required(VERSION 3.15)
set(project_name osal-windows)
project(${project_name} C CXX ASM)

set(include_files
	include/osal-windows/osal-windows.h
)

set(source_files
	source/osal-windows.cpp
)

add_library(${project_name} ${include_files} ${source_files})
target_include_directories(${project_name} PUBLIC include)

target_link_libraries(${project_name} osal-core)

file(GLOB_RECURSE lib_files "${PROJECT_BINARY_DIR}/lib/*")
install(TARGETS ${project_name} RUNTIME DESTINATION bin)
install(FILES ${lib_files} DESTINATION lib)