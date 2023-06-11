add_executable(RobotArm
	"src/main.cpp"
	"src/draw.cpp"
)
target_compile_features(RobotArm PRIVATE cxx_std_20)
target_link_libraries(RobotArm PRIVATE CGFramework)
enable_sanitizers(RobotArm)
set_project_warnings(RobotArm)

target_compile_definitions(RobotArm PRIVATE "-DDATA_DIR=\"${CMAKE_CURRENT_LIST_DIR}/data/\"")
