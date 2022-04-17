set(project_name feature-detector)

set(include_files
	include/feature-detector/feature-detector.h
)

set(source_files
	src/feature-detector.cpp
)

set(cli-files
	src/cli/HarrisCornerDetector.h
	src/cli/ShiTomasiCornerDetector.h
	src/cli/SIFT.h
	src/cli/SURF.h
	src/cli/FAST.h
	src/cli/BRIEF.h
	src/cli/ORB.h
	src/cli/BRISK.h
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
target_link_libraries(${project_name} CONAN_PKG::opencv)
target_link_libraries(${project_name} CONAN_PKG::cxxopts)

file(GLOB_RECURSE lib_files "${PROJECT_BINARY_DIR}/lib/*")
install(TARGETS ${project_name} RUNTIME DESTINATION bin)
install(FILES ${lib_files} DESTINATION lib)

add_executable(${project_name}-cli ${cli-files})
target_link_libraries(${project_name}-cli ${project_name})