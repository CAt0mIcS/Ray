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
	"Core/RSurface.h"
	"Core/RImage.h"
	"Core/RImageView.h"
	"Core/RSwapchain.h"
	"Core/RSampler.h"

	# ------------------------------------------------------------------
	# Synchronization
	"Synchronization/RFence.h"
	"Synchronization/RSemaphore.h"

	# ------------------------------------------------------------------
	# Pipeline
	"Pipeline/RPipeline.h"
	"Pipeline/RDescriptor.h"
	"Pipeline/RUniform.h"
	"Pipeline/RShader.h"
)

find_host_package(PythonInterp 3 REQUIRED FATAL_ERROR)

add_library("RayRenderer" "${SRC}")
AddRecursiveFilters("${SRC}")

add_subdirectory("OpenGL")
add_subdirectory("Vulkan")


