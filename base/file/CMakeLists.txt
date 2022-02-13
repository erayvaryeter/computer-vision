set(project_name file)

set(include_files
	include/file/file.h
)

set(source_files
	src/file.cpp
)

set(test_files
    test/main.cpp
	test/file-test.cpp
)

add_library(${project_name} ${include_files} ${source_files})
target_include_directories(${project_name} PUBLIC include)
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:4099")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:2005")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:4996")
target_link_libraries(${project_name} string)
target_link_libraries(${project_name} assertion)

file(GLOB_RECURSE lib_files "${PROJECT_BINARY_DIR}/lib/*")
install(TARGETS ${project_name} RUNTIME DESTINATION bin)
install(FILES ${lib_files} DESTINATION lib)


enable_testing()
add_executable(${project_name}-test ${test_files})
target_link_libraries(${project_name}-test ${project_name})
target_link_libraries(${project_name}-test CONAN_PKG::catch2)

if(MSVC)
  target_compile_options(${project_name}-test PRIVATE)
else()
  target_compile_options(${project_name}-test PRIVATE)
endif()

if(${file-unittest})
    add_custom_command(
         TARGET ${project_name}-test
         COMMENT "Run file tests"
         POST_BUILD 
         COMMAND ${project_name}-test
    )
endif()