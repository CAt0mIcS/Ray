set(SRC
	"main.cpp"
)

add_executable("PhysxSetup" "${SRC}")
target_include_directories("PhysxSetup" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("PhysxSetup" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("PhysxSetup" PRIVATE "Ray")

set_target_properties("PhysxSetup" PROPERTIES FOLDER "Tests")

SetOutputDirectory("PhysxSetup" "")
SetWorkingDirectoryPlane("PhysxSetup" "${CMAKE_SOURCE_DIR}")