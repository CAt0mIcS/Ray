set(SRC
	"Rpch.h"

	# ------------------------------------------------------------------
	# Shared files
	"../Core/RCore.h"
	"../Core/RCore.c"
	"../Core/RUtils.h"
	"../Core/RUtils.c"
	"../RBase.h"


	# ------------------------------------------------------------------
	# Files required for initialization
	"Core/RVulkanCore.c"
	"Core/RVulkanUtils.c"
)

# ------------------------------------------------------------------
# Find required packages
find_package("Vulkan" REQUIRED FATAL_ERROR)

set(PUBLIC_INC
	"${CMAKE_SOURCE_DIR}"
	"${Vulkan_INCLUDE_DIRS}"
)

set(PRIVATE_INC
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
	"${Vulkan_LIBRARIES}"
)


# ------------------------------------------------------------------
# Setup
add_library("RayRendererVulkan" SHARED "${SRC}")
target_include_directories("RayRendererVulkan"
	PUBLIC
		"${PUBLIC_INC}"
	PRIVATE
		"${PRIVATE_INC}"
)
target_link_libraries("RayRendererVulkan"
	PUBLIC
		"${PUBLIC_LIB}"
	PRIVATE
		"${PRIVATE_LIB}"
)
target_compile_definitions("RayRendererVulkan" PRIVATE "RR_BUILD")
target_precompile_headers("RayRendererVulkan" PRIVATE "Rpch.h")

AddRecursiveFilters("${SRC}")
SetOutputDirectory("RayRendererVulkan" "")
SetWorkingDirectoryPlane("RayRendererVulkan" "${CMAKE_SOURCE_DIR}")




