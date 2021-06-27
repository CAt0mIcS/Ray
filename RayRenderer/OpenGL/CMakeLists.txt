set(SRC
	"Rpch.h"

	# ------------------------------------------------------------------
	# Shared files
	"../Core/RCore.h"
	"../Core/RCore.c"
	"../Core/RUtils.h"
	"../Core/RUtils.c"
	"../Core/RCommandPool.h"
	"../Core/RLogicalDevice.h"
	"../RBase.h"

	# ------------------------------------------------------------------
	# Files required for initialization
	"Core/ROpenGLCore.c"
	"Core/ROpenGLUtils.c"
	"Core/ROpenGLCommandPool.c"
)

find_package (Python COMPONENTS Interpreter)

set(PUBLIC_INC
	"${CMAKE_SOURCE_DIR}"
)

set(PRIVATE_INC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_SOURCE_DIR}/Extern/Glad/include"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
	"Glad"
)


# ------------------------------------------------------------------
# Setup
add_library("RayRendererOpenGL" SHARED "${SRC}")
target_include_directories("RayRendererOpenGL"
	PUBLIC
		"${PUBLIC_INC}"
	PRIVATE
		"${PRIVATE_INC}"
)
target_link_libraries("RayRendererOpenGL"
	PUBLIC
		"${PUBLIC_LIB}"
	PRIVATE
		"${PRIVATE_LIB}"
)
target_compile_definitions("RayRendererOpenGL" PRIVATE "RR_BUILD")
target_precompile_headers("RayRendererOpenGL" PRIVATE "Rpch.h")

AddRecursiveFilters("${SRC}")
SetOutputDirectory("RayRendererOpenGL" "")
SetWorkingDirectoryPlane("RayRendererOpenGL" "${CMAKE_SOURCE_DIR}")

# Run auto loader generator
add_custom_command(TARGET "RayRendererOpenGL" POST_BUILD
	COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Scripts/generate_loader.py ${CMAKE_SOURCE_DIR}
)


