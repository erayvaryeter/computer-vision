set(project_name code-coverage)

set(test_files
	src/main.cpp
	../../base/assertion/test/assertion-test.cpp
	../../base/datetime/test/datetime-test.cpp
	../../base/file/test/file-test.cpp
	../../base/logger/test/logger-test.cpp
	../../base/string/test/string-test.cpp
	../../image-processing/file-storage/test/file-storage-test.cpp
	../../image-processing/histogram/test/histogram-test.cpp
)

enable_testing()
add_executable(${project_name} ${test_files})
target_link_libraries(${project_name} assertion)
target_link_libraries(${project_name} datetime)
target_link_libraries(${project_name} file)
target_link_libraries(${project_name} logger)
target_link_libraries(${project_name} string)
target_link_libraries(${project_name} histogram)
target_link_libraries(${project_name} CONAN_PKG::catch2)

if(MSVC)
  target_compile_options(${project_name} PRIVATE)
else()
  target_compile_options(${project_name} PRIVATE)
endif()

file(GLOB_RECURSE bin_files "${PROJECT_BINARY_DIR}/bin/*")
install(TARGETS ${project_name} RUNTIME DESTINATION bin)
install(FILES ${bin_files} DESTINATION bin)