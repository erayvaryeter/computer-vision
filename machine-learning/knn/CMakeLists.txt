set(project_name knn)

set(include_files
	include/knn/knn.h
	include/knn/parameters.h
)

set(source_files
	src/knn.cpp
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
target_link_libraries(${project_name} feature-matcher)
target_link_libraries(${project_name} pca)
target_link_libraries(${project_name} preprocessing)
target_link_libraries(${project_name} CONAN_PKG::opencv)
target_link_libraries(${project_name} CONAN_PKG::cxxopts)

file(GLOB_RECURSE lib_files "${PROJECT_BINARY_DIR}/lib/*")
install(TARGETS ${project_name} RUNTIME DESTINATION bin)
install(FILES ${lib_files} DESTINATION lib)

add_executable(${project_name}-cli ${cli-files})
target_link_libraries(${project_name}-cli ${project_name})