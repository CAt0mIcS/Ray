set(SRC
	"Core/RCore.h"
	"Core/RCore.c"
	"Core/RUtils.h"
	"Core/RUtils.c"
	"Core/RInstance.h"
	"Core/RPhysicalDevice.h"
	"Core/RLogicalDevice.h"
	"Core/RDeviceMemory.h"
	"Core/RBuffer.h"
	"Core/RCommandPool.h"
	"Core/RCommandBuffer.h"
	"Core/RRenderPass.h"
	"Core/RFramebuffer.h"

	# ------------------------------------------------------------------
	# Synchronization
	"Synchronization/RFence.h"
	"Synchronization/RSemaphore.h"
)

add_library("RayRendererBase" "${SRC}")
AddRecursiveFilters("${SRC}")

add_subdirectory("OpenGL")
add_subdirectory("Vulkan")


