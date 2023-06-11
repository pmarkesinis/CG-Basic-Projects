cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(ComputerGraphics CXX)

set(FRAMEWORK_BASIC_LIBRARY TRUE)
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/framework")
	# Create framework library and include CMake scripts (compiler warnings, sanitizers and static analyzers).
	add_subdirectory("framework")
else()
	# During development the framework lives in parent folder.
	add_subdirectory("../../../framework/" "${CMAKE_BINARY_DIR}/framework/")
endif()

add_executable(Practical1 "src/main.cpp")
target_compile_features(Practical1 PRIVATE cxx_std_20)
target_link_libraries(Practical1 PRIVATE CGFramework)
set_project_warnings(Practical1)
enable_sanitizers(Practical1)

if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/grading_tests/")
	add_subdirectory("grading_tests")
endif()	

