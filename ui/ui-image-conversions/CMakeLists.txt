set(project_name ui-image-conversions)

set(include_files
	include/ui-image-conversions/ui-image-conversions.h
)

set(source_files
	src/ui-image-conversions.cpp
)

set(test_files
    test/main.cpp
	test/ui-image-conversions-test.cpp
	test/Fixture.h
)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_library(${project_name} ${include_files} ${source_files})
target_include_directories(${project_name} PUBLIC include)

set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:4099")

target_link_libraries(${project_name} file)
target_link_libraries(${project_name} CONAN_PKG::opencv-cuda)
target_link_libraries(${project_name} Qt5::Widgets)

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