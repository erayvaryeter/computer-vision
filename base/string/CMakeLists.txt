set(project_name string)

set(include_files
	include/string/string.h
)

set(source_files
	src/string.cpp
)

set(test_files
    test/main.cpp
	test/string-test.cpp
)

add_library(${project_name} ${include_files} ${source_files})
target_include_directories(${project_name} PUBLIC include)
target_link_libraries(${project_name} logger)
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:4099")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:2005")

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

if(${string-unittest})
    add_custom_command(
         TARGET ${project_name}-test
         COMMENT "Run string tests"
         POST_BUILD 
         COMMAND ${project_name}-test
    )
endif()