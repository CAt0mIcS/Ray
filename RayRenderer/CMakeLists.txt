set(SRC
	"Rpch.h"
	"RBase.h"

	# ------------------------------------------------------------------
	# Files required for initialization
	"Core/RCore.h"
	"Core/RUtils.h"

	# ------------------------------------------------------------------
	# Vulkan specific files
	"Vulkan/Core/RVulkanCore.c"
	"Vulkan/Core/RVulkanUtils.c"

	# ------------------------------------------------------------------
	# OpenGL specific files
	"OpenGL/Core/ROpenGLCore.c"
	"OpenGL/Core/ROpenGLUtils.c"
)

# ------------------------------------------------------------------
# Find required packages
find_package("Vulkan" REQUIRED FATAL_ERROR)

set(PUBLIC_INC
	"${Vulkan_INCLUDE_DIRS}"
	"${CMAKE_SOURCE_DIR}"
)

set(PRIVATE_INC
	"${CMAKE_CURRENT_SOURCE_DIR}"
	"${CMAKE_SOURCE_DIR}/Extern"
	"${CMAKE_SOURCE_DIR}/Extern/Glad/include"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
	"${Vulkan_LIBRARIES}"
	"opengl32"
	"Glad"
)


# ------------------------------------------------------------------
# Setup
add_library("RayRenderer" SHARED "${SRC}")
target_include_directories("RayRenderer"
	PUBLIC
		"${PUBLIC_INC}"
	PRIVATE
		"${PRIVATE_INC}"
)
target_link_libraries("RayRenderer"
	PUBLIC
		"${PUBLIC_LIB}"
	PRIVATE
		"${PRIVATE_LIB}"
)
target_compile_definitions("RayRenderer" PRIVATE "RR_BUILD")
target_precompile_headers("RayRenderer" PRIVATE "Rpch.h")

AddRecursiveFilters("${SRC}")
SetOutputDirectory("RayRenderer" "")
SetWorkingDirectoryPlane("RayRenderer" "${CMAKE_SOURCE_DIR}")


