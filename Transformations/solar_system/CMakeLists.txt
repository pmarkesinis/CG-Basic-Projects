find_package(OpenGL REQUIRED)

add_executable(SolarSystem
	"src/draw.cpp"
	"src/main.cpp" "src/reference_saver.h")
target_compile_features(SolarSystem PRIVATE cxx_std_20)
target_link_libraries(SolarSystem PRIVATE CGFramework stb OpenGL::GLU)
enable_sanitizers(SolarSystem)
set_project_warnings(SolarSystem)

target_compile_definitions(SolarSystem PRIVATE "-DDATA_DIR=\"${CMAKE_CURRENT_LIST_DIR}/data/\"")

add_subdirectory("tests")
