set(SRC
	"src/glad.c"
	"include/glad/glad.h"
	"include/KHR/khrplatform.h"
)

add_library("Glad" SHARED "${SRC}")
set_target_properties("Glad" PROPERTIES FOLDER "extern")
target_include_directories("Glad" PUBLIC "include")

target_compile_definitions("Glad" PRIVATE "GLAD_GLAPI_EXPORT_BUILD")

AddRecursiveFilters("${SRC}")

SetOutputDirectory("Glad" "")


