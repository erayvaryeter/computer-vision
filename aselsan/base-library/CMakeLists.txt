project(base-library)

set(include_files
	include/base-library/base-library.h
)

set(source_files
	src/base-library.cpp
)

add_library(${project_name} ${include_files} ${source_files})
target_include_directories(${project_name} PUBLIC include)
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:4099")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:2005")

file(GLOB_RECURSE lib_files "${PROJECT_BINARY_DIR}/lib/*")
install(TARGETS ${project_name} RUNTIME DESTINATION bin)
install(FILES ${lib_files} DESTINATION lib)