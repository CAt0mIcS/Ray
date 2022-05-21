set(SRC
	"main.cpp"
)

add_executable("Lighting" "${SRC}")
target_include_directories("Lighting" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("Lighting" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("Lighting" PRIVATE "Ray")

set_target_properties("Lighting" PROPERTIES FOLDER "Tests")

SetOutputDirectory("Lighting" "")
SetWorkingDirectoryPlane("Lighting" "${CMAKE_SOURCE_DIR}")