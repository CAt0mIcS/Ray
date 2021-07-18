set(SRC
	"Rpch.h"

	# ------------------------------------------------------------------
	# Core files
	"Core/ROpenGLInstance.c"
	"Core/ROpenGLPhysicalDevice.c"
	"Core/ROpenGLLogicalDevice.c"
	"Core/ROpenGLUtils.c"
	"Core/ROpenGLDeviceMemory.c"
	"Core/ROpenGLBuffer.c"
	"Core/ROpenGLCommandPool.c"
	"Core/ROpenGLCommandBuffer.c"
	"Core/ROpenGLRenderPass.c"
	"Core/ROpenGLFramebuffer.c"
	"Core/ROpenGLSurface.c"
	"Core/ROpenGLImage.c"
	"Core/ROpenGLImageView.c"
	"Core/ROpenGLSwapchain.c"

	# ------------------------------------------------------------------
	# Synchronization
	"Synchronization/ROpenGLFence.c"
	"Synchronization/ROpenGLSemaphore.c"

	# ------------------------------------------------------------------
	# Pipeline
	"Pipeline/ROpenGLPipeline.c"
	"Pipeline/ROpenGLUniform.c"
	"Pipeline/ROpenGLDescriptor.c"
	"Pipeline/ROpenGLShader.c"
)

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
	"RayRenderer"
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
	COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Scripts/generate_loader.py ${CMAKE_SOURCE_DIR}
)