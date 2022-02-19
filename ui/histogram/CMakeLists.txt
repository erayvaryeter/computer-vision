set(project_name histogram-ui)

set(form_files
	form/QtWidgetsApplication1.ui
)

set(include_files
	include/histogram/QtWidgetsApplication1.h
)

set(source_files
	src/main.cpp
	src/QtWidgetsApplication1.cpp
)

set(resource_files
	resource/QtWidgetsApplication1.qrc
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "C:/Qt5/5.15.2/msvc2019_64/lib/cmake/Qt5")
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_LIST_DIR}/form")

file(COPY "${CMAKE_CURRENT_LIST_DIR}/../platform-files/" DESTINATION "${PROJECT_BINARY_DIR}/bin")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(${project_name} ${form_files} ${include_files} ${source_files} ${resource_files})
target_include_directories(${project_name} PUBLIC include)

set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:4099")

target_link_libraries(${project_name} histogram)
target_link_libraries(${project_name} Qt5::Widgets)

file(GLOB_RECURSE bin_files "${PROJECT_BINARY_DIR}/bin/*")
install(TARGETS ${project_name} RUNTIME DESTINATION bin)
install(FILES ${bin_files} DESTINATION bin)