cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(ComputerGraphics C CXX)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/framework")
	# Create framework library and include CMake scripts (compiler warnings, sanitizers and static analyzers).
	add_subdirectory("framework")
else()
	# During development the framework lives in parent folder.
	add_subdirectory("../../../framework/" "${CMAKE_BINARY_DIR}/framework/")
endif()

find_package(OpenGL REQUIRED)

add_library(Assignment4Lib STATIC "src/ray_tracing.cpp" "src/scene.cpp" )
target_link_libraries(Assignment4Lib PUBLIC CGFramework)
target_compile_features(Assignment4Lib PUBLIC cxx_std_20)
set_project_warnings(Assignment4Lib)
enable_sanitizers(Assignment4Lib)

add_executable(Assignment4 "src/main.cpp" "src/draw.cpp")
target_link_libraries(Assignment4 PRIVATE Assignment4Lib CGFramework OpenGL::GLU)
target_compile_features(Assignment4 PRIVATE cxx_std_20)
enable_sanitizers(Assignment4)
set_project_warnings(Assignment4)

target_compile_definitions(Assignment4 PRIVATE "-DGL_SILENCE_DEPRECATION=1")
target_compile_definitions(Assignment4 PRIVATE "-DDATA_DIR=\"${CMAKE_CURRENT_LIST_DIR}/data/\"" "-DOUTPUT_DIR=\"${CMAKE_CURRENT_LIST_DIR}/\"")

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/grading_tests/")
	add_subdirectory("grading_tests")
endif()
