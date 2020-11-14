set(SRC
	"src/glad.c"
	"include/glad/glad.h"
	"include/KHR/khrplatform.h"
)

add_library("Glad" STATIC "${SRC}")
set_target_properties("Glad" PROPERTIES FOLDER "extern")
AddRecursiveFilters("${SRC}")


