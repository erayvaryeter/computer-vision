﻿set(project_name instance-segmentation)

set(include_files
	include/instance-segmentation/instance-segmentation.h
)

set(source_files
	src/instance-segmentation.cpp
)

set(cli-files
	src/cli/main.cpp
)

add_library(${project_name} ${include_files} ${source_files})
target_include_directories(${project_name} PUBLIC include)
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:4099")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:2005")

target_link_libraries(${project_name} string)
target_link_libraries(${project_name} assertion)
target_link_libraries(${project_name} file)
target_link_libraries(${project_name} object-detection)
target_link_libraries(${project_name} age-estimator)
target_link_libraries(${project_name} gender-estimator)
target_link_libraries(${project_name} CONAN_PKG::opencv)
target_link_libraries(${project_name} CONAN_PKG::cxxopts)

file(GLOB_RECURSE lib_files "${PROJECT_BINARY_DIR}/lib/*")
install(TARGETS ${project_name} RUNTIME DESTINATION bin)
install(FILES ${lib_files} DESTINATION lib)

add_executable(${project_name}-cli ${cli-files})
target_link_libraries(${project_name}-cli ${project_name})