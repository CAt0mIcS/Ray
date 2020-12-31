set(SRC
	"src/Sandbox.h"
	"src/Sandbox.cpp"
	"src/pch.h"
)

set(PUBLIC_INC
	""
)

set(PRIVATE_INC
	""
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
	"Ray"
)


add_executable("RayEditor" WIN32 "${SRC}")
target_include_directories("RayEditor"
	PUBLIC
		"${PUBLIC_INC}"
	PRIVATE
		"${PRIVATE_INC}"
)

target_link_libraries("RayEditor"
	PUBLIC
		"${PUBLIC_LIB}"
	PRIVATE
		"${PRIVATE_LIB}"
)

target_precompile_headers("RayEditor" PRIVATE "src/pch.h")
AddRecursiveFilters("${SRC}")
SetOutputDirectory("RayEditor" "")
SetWorkingDirectory("RayEditor" "")

