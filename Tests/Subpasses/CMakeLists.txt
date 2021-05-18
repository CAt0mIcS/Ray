set(SRC
	"main.cpp"
)

add_executable("Subpasses" "${SRC}")
target_include_directories("Subpasses" PRIVATE "${CMAKE_SOURCE_DIR}/Sources")
target_compile_definitions("Subpasses" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("Subpasses" PRIVATE "Ray")

set_target_properties("Subpasses" PROPERTIES FOLDER "Tests")

SetOutputDirectory("Subpasses" "")
SetWorkingDirectoryPlane("Subpasses" "${CMAKE_SOURCE_DIR}")
