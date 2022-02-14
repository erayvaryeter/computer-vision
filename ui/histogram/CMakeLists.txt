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

add_executable(${project_name} ${include_files} ${source_files} ${form_files} ${resource_files})
target_include_directories(${project_name} PUBLIC include)

set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:4099")
set_target_properties(${project_name} PROPERTIES LINK_FLAGS "/ignore:2005")

target_link_libraries(${project_name} histogram)
target_link_libraries(${project_name} CONAN_PKG::qt)

find_package(ZLIB CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(PNG CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(freetype CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(BZip2 CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(double-conversion CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(pcre2 CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(libelf CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(Iconv CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(Intl CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(PCRE CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(libffi CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(glib CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(harfbuzz CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(Brotli CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(OpenSSL CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(PostgreSQL CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(SQLite3 CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})
find_package(Qt6 CONFIG REQUIRED PATHS ${CMAKE_BINARY_DIR})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

file(GLOB_RECURSE bin_files "${PROJECT_BINARY_DIR}/bin/*")
install(TARGETS ${project_name} RUNTIME DESTINATION bin)
install(FILES ${bin_files} DESTINATION bin)