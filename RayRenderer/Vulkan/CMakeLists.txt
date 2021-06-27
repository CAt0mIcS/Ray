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
	"Core/RVulkanCore.c"
	"Core/RVulkanUtils.c"
	"Core/RVulkanCommandPool.c"
)

# ------------------------------------------------------------------
# Find required packages
find_package("Vulkan" REQUIRED FATAL_ERROR)
find_package (Python COMPONENTS Interpreter)

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

# Run auto loader generator
add_custom_command(TARGET "RayRendererVulkan" POST_BUILD
	COMMAND ${Python_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Scripts/generate_loader.py ${CMAKE_SOURCE_DIR}
)


