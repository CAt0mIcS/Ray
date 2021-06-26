set(SRC 
	"include/KHR/khrplatform.h"
	"include/glad/glad.h"
	"src/glad.c"
)

add_library("Glad" "${SRC}")
target_include_directories("Glad" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
AddRecursiveFilters("${SRC}")
SetOutputDirectory("Glad" "")
SetWorkingDirectoryPlane("Glad" "${CMAKE_SOURCE_DIR}")