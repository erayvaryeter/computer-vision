set(project_name tracking)

set(include_files
	include/tracking/tracking.h
)

set(source_files
	src/tracking.cpp
)

set(face-tracking-cli-files
	src/cli/FaceTracking.cpp
)

set(instance-segmentation-tracking-cli-files
	src/cli/InstanceSegmentationTracking.cpp
)

add_library(${project_name} ${include_files} ${source_files})
target_include_directories(${project_name} PUBLIC include)
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:4099")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:2005")

target_link_libraries(${project_name} string)
target_link_libraries(${project_name} assertion)
target_link_libraries(${project_name} file)
target_link_libraries(${project_name} face-detection)
target_link_libraries(${project_name} instance-segmentation)
target_link_libraries(${project_name} CONAN_PKG::opencv)
target_link_libraries(${project_name} CONAN_PKG::cxxopts)

file(GLOB_RECURSE lib_files "${PROJECT_BINARY_DIR}/lib/*")
install(TARGETS ${project_name} RUNTIME DESTINATION bin)
install(FILES ${lib_files} DESTINATION lib)

add_executable(face-tracking-cli ${face-tracking-cli-files})
target_link_libraries(face-tracking-cli ${project_name})

add_executable(instance-segmentation-tracking-cli ${instance-segmentation-tracking-cli-files})
target_link_libraries(instance-segmentation-tracking-cli ${project_name})