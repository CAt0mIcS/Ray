set(SRC
	"Core/RCore.h"
	"Core/RCore.c"
	"Core/RUtils.h"
	"Core/RUtils.c"
	"Core/RInstance.h"
	"Core/RPhysicalDevice.h"
	"Core/RLogicalDevice.h"
	"Core/RCommandPool.h"
)

add_library("RayRendererBase" "${SRC}")
AddRecursiveFilters("${SRC}")

add_subdirectory("OpenGL")
add_subdirectory("Vulkan")


