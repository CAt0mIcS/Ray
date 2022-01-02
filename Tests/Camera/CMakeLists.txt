set(SHADERS
	"Shaders/Orthographic.vert"
	"Shaders/Orthographic.frag"
)

set(SRC
	"main.cpp"
)

add_executable("Camera" "${SRC}")
target_include_directories("Camera" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("Camera" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("Camera" PRIVATE "Ray")

set_target_properties("Camera" PROPERTIES FOLDER "Tests")

SetOutputDirectory("Camera" "")
SetWorkingDirectoryPlane("Camera" "${CMAKE_SOURCE_DIR}")

AddRecursiveFilters(${SHADERS})