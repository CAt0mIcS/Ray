set(SRC
	"main.cpp"
)

add_executable("ShaderDataTypes" "${SRC}")
target_include_directories("ShaderDataTypes" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("ShaderDataTypes" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("ShaderDataTypes" PRIVATE "Ray")

set_target_properties("ShaderDataTypes" PROPERTIES FOLDER "Tests")

SetOutputDirectory("ShaderDataTypes" "")
SetWorkingDirectoryPlane("ShaderDataTypes" "${CMAKE_SOURCE_DIR}")