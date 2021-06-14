set(SRC
	"main.cpp"
)

add_executable("ShaderReflection" "${SRC}")
target_include_directories("ShaderReflection" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("ShaderReflection" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("ShaderReflection" PRIVATE "Ray")

set_target_properties("ShaderReflection" PROPERTIES FOLDER "Tests")

SetOutputDirectory("ShaderReflection" "")
SetWorkingDirectoryPlane("ShaderReflection" "${CMAKE_SOURCE_DIR}")
