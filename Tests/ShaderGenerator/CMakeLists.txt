set(SRC
	"main.cpp"
)

add_executable("ShaderGenerator" "${SRC}")
target_include_directories("ShaderGenerator" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("ShaderGenerator" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("ShaderGenerator" PRIVATE "Ray")

set_target_properties("ShaderGenerator" PROPERTIES FOLDER "Tests")

SetOutputDirectory("ShaderGenerator" "")
SetWorkingDirectoryPlane("ShaderGenerator" "${CMAKE_SOURCE_DIR}")
