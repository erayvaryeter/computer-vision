set(project_name val-set-creator)

set(source_files
	main.cpp
)

add_executable(${project_name} ${source_files})
target_link_libraries(${project_name} string)
target_link_libraries(${project_name} file)