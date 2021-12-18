set(SRC
	"main.cpp"
	"Gizmo.h"
	"Gizmo.cpp"
)

add_executable("Raditor" "${SRC}")
target_include_directories("Raditor" PRIVATE "${CMAKE_SOURCE_DIR}")
target_compile_definitions("Raditor" PRIVATE "ENTT_API_IMPORT")
target_link_libraries("Raditor" PRIVATE "Ray")

AddRecursiveFilters("${SRC}")
SetOutputDirectory("Raditor" "")
SetWorkingDirectoryPlane("Raditor" "${CMAKE_SOURCE_DIR}")