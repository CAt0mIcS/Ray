set(SRC
	"Rpch.h"

	# ------------------------------------------------------------------
	# Core files
	"Core/RVulkanInstance.c"
	"Core/RVulkanPhysicalDevice.c"
	"Core/RVulkanLogicalDevice.c"
	"Core/RVulkanUtils.c"
	"Core/RVulkanDeviceMemory.c"
	"Core/RVulkanBuffer.c"
	"Core/RVulkanCommandPool.c"
	"Core/RVulkanCommandBuffer.c"
	"Core/RVulkanRenderPass.c"
	"Core/RVulkanFramebuffer.c"
	"Core/RVulkanSurface.c"
	"Core/RVulkanImage.c"

	# ------------------------------------------------------------------
	# Synchronization
	"Synchronization/RVulkanFence.c"
	"Synchronization/RVulkanSemaphore.c"

	# ------------------------------------------------------------------
	# Pipeline
	"Pipeline/RVulkanPipeline.c"
	"Pipeline/RVulkanUniform.c"
	"Pipeline/RVulkanDescriptor.c"
	"Pipeline/RVulkanShader.c"
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
	"RayRenderer"
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
	COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Scripts/generate_loader.py ${CMAKE_SOURCE_DIR}
)
