set(project_name utk-face-data-preparation)

set(include_files
)

set(source_files
	src/main.cpp
)

set(cli-files
)

add_executable(${project_name} ${include_files} ${source_files})

target_link_libraries(${project_name} string)
target_link_libraries(${project_name} assertion)
target_link_libraries(${project_name} file)
target_link_libraries(${project_name} CONAN_PKG::opencv)

install(TARGETS ${project_name} RUNTIME DESTINATION bin)