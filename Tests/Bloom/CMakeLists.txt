set(SHADERS
	"Shaders/Bloom.vert"
	"Shaders/Bloom.frag"
)

set(SRC
	"main.cpp"

	${SHADERS}
)

add_executable("Bloom" "${SRC}")
target_include_directories("Bloom" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("Bloom" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("Bloom" PRIVATE "Ray")

set_target_properties("Bloom" PROPERTIES FOLDER "Tests")

SetOutputDirectory("Bloom" "")
SetWorkingDirectoryPlane("Bloom" "${CMAKE_SOURCE_DIR}")

AddRecursiveFilters("${SHADERS}")

