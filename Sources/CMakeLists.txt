set(SHADERS
	"../Resources/Shaders/TexturedShader.vert"
	"../Resources/Shaders/TexturedShader.frag"
	"../Resources/Shaders/DefaultShader.vert"
	"../Resources/Shaders/DefaultShader.frag"
	"../Resources/Shaders/ModelShader.vert"
	"../Resources/Shaders/ModelShader.frag"
	"../Resources/Shaders/ModelShader_Spec.vert"
	"../Resources/Shaders/ModelShader_Spec.frag"
	"../Resources/Shaders/ModelShader_Diff.vert"
	"../Resources/Shaders/ModelShader_Diff.frag"
	"../Resources/Shaders/ModelShader_DiffSpec.vert"
	"../Resources/Shaders/ModelShader_DiffSpec.frag"
)

set(SRC
	"Rpch.h"
	"RBase.h"

	# ------------------------------------------------------------------
	# Scene
	"Scene/REntity.h"
	"Scene/RScene.h"
	"Scene/RScene.cpp"
	"Scene/RCamera.h"
	"Scene/RCamera.cpp"

	# ------------------------------------------------------------------
	# Core classes
	"Core/REngine.h"
	"Core/REngine.cpp"
	"Core/RTime.h"
	"Core/RTime.cpp"
	"Core/RMath.h"
	"Core/RMath.cpp"
	"Core/RKeyCodes.h"
	"Core/RComponent.h"
	
	# ------------------------------------------------------------------
	# Devices
	"Devices/RWindow.h"
	"Devices/RWindow.cpp"
	"Devices/RMouse.h"
	"Devices/RMouse.cpp"
	"Devices/RKeyboard.h"
	"Devices/RKeyboard.cpp"

	# ------------------------------------------------------------------
	# Graphics
	"Graphics/RGraphics.h"
	"Graphics/RGraphics.cpp"
	"Graphics/RVertex.h"
	"Graphics/RCodex.h"
	
	"Graphics/Core/RVulkanInstance.h"
	"Graphics/Core/RVulkanInstance.cpp"
	"Graphics/Core/RPhysicalDevice.h"
	"Graphics/Core/RPhysicalDevice.cpp"
	"Graphics/Core/RSurface.h"
	"Graphics/Core/RSurface.cpp"
	"Graphics/Core/RLogicalDevice.h"
	"Graphics/Core/RLogicalDevice.cpp"
	"Graphics/Core/RSwapchain.h"
	"Graphics/Core/RSwapchain.cpp"
	"Graphics/Core/RBindable.h"
	"Graphics/Core/RBindable.cpp"

	"Graphics/Pipelines/RPipeline.h"
	"Graphics/Pipelines/RPipeline.cpp"
	"Graphics/Pipelines/RGraphicsPipeline.h"
	"Graphics/Pipelines/RGraphicsPipeline.cpp"
	"Graphics/Pipelines/RComputePipeline.h"
	"Graphics/Pipelines/RComputePipeline.cpp"
	"Graphics/Pipelines/RShader.h"
	"Graphics/Pipelines/RShader.cpp"

	"Graphics/Pipelines/Uniforms/RDescriptor.h"
	"Graphics/Pipelines/Uniforms/RDescriptor.cpp"
	"Graphics/Pipelines/Uniforms/RUniform.h"
	"Graphics/Pipelines/Uniforms/RUniform.cpp"
	"Graphics/Pipelines/Uniforms/RBufferUniform.h"
	"Graphics/Pipelines/Uniforms/RBufferUniform.cpp"
	"Graphics/Pipelines/Uniforms/RSamplerUniform.h"
	"Graphics/Pipelines/Uniforms/RSamplerUniform.cpp"

	"Graphics/Buffers/RBuffer.h"
	"Graphics/Buffers/RBuffer.cpp"
	"Graphics/Buffers/RIndexBuffer.h"
	"Graphics/Buffers/RIndexBuffer.cpp"
	"Graphics/Buffers/RUniformBuffer.h"
	"Graphics/Buffers/RUniformBuffer.cpp"
	"Graphics/Buffers/RVertexBuffer.h"
	"Graphics/Buffers/RVertexBuffer.cpp"
	"Graphics/Buffers/RFramebuffer.h"
	"Graphics/Buffers/RFramebuffer.cpp"
	"Graphics/Buffers/RBufferSynchronizer.h"
	"Graphics/Buffers/RBufferSynchronizer.cpp"
	"Graphics/Buffers/RDynamicBuffer.h"
	"Graphics/Buffers/RDynamicBuffer.cpp"

	"Graphics/Images/RImage.h"
	"Graphics/Images/RImage.cpp"
	"Graphics/Images/RImage2D.h"
	"Graphics/Images/RImage2D.cpp"
	"Graphics/Images/RDepthImage.h"
	"Graphics/Images/RDepthImage.cpp"
	"Graphics/Images/RImageView.h"
	"Graphics/Images/RImageView.cpp"
	"Graphics/Images/RTexture.h"
	"Graphics/Images/RTexture.cpp"
	"Graphics/Images/RTexture2D.h"
	"Graphics/Images/RTexture2D.cpp"
	"Graphics/Images/RTextureSampler.h"
	"Graphics/Images/RTextureSampler.cpp"

	"Graphics/Commands/RCommandBuffer.h"
	"Graphics/Commands/RCommandBuffer.cpp"
	"Graphics/Commands/RCommandPool.h"
	"Graphics/Commands/RCommandPool.cpp"

	"Graphics/RenderPass/RRenderPass.h"
	"Graphics/RenderPass/RRenderPass.cpp"
	
	"Graphics/Renderers/RRenderer.h"
	"Graphics/Renderers/RRenderer.cpp"
	"Graphics/Renderers/RSubRenderer.cpp"
	"Graphics/Renderers/RSubRenderer.h"
	"Graphics/Renderers/RMeshRenderer.cpp"
	"Graphics/Renderers/RMeshRenderer.h"

	# ------------------------------------------------------------------
	# Geometry
	"Geometry/RMesh.h"
	"Geometry/RMesh.cpp"
	"Geometry/Registry/RGeometricPrimitives.h"
	"Geometry/Registry/RGeometricPrimitives.cpp"
	"Geometry/Registry/RMaterial.h"
	"Geometry/Registry/RMaterial.cpp"
	"Geometry/Registry/RModel.h"
	"Geometry/Registry/RModel.cpp"

	# ------------------------------------------------------------------
	# Utilities
	"Utils/RSerialize.inl"
	"Utils/RString.h"
	"Utils/RString.cpp"
	"Utils/RThreadPool.h"
	"Utils/RThreadPool.cpp"
	"Utils/RNonCopyable.h"
	"Utils/RLogger.h"
	"Utils/RLogger.cpp"
	"Utils/RAssert.h"
	"Utils/RQueue.h"
	"Utils/RException.h"
	"Utils/RException.cpp"

	# ------------------------------------------------------------------
	# Event system
	"Events/REngineEvents.h"
	"Events/RMouseEvents.h"
	"Events/RKeyboardEvents.h"
	"Events/REventDispatcher.h"
	"Events/REventListener.h"

	# ------------------------------------------------------------------
	# ImGui
	"ImGui/RImGui.h"
	"ImGui/RImGui.cpp"

	"${SHADERS}"
)

# ------------------------------------------------------------------
# Find required packages
find_package("Vulkan" REQUIRED FATAL_ERROR)

set(PUBLIC_INC
    "${Vulkan_INCLUDE_DIRS}"
)

set(PRIVATE_INC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/Extern/entt/src"
    "${CMAKE_SOURCE_DIR}/Extern/glfw/include"
    "${CMAKE_SOURCE_DIR}/Extern"
    "${CMAKE_SOURCE_DIR}/Extern/Ion"
)

set(PUBLIC_LIB
    "glm"
)

set(PRIVATE_LIB
    "glfw"
    "${Vulkan_LIBRARIES}"
    "assimp"
    "glslang"
    "SPIRV"
    "Ion"
    "ImGui"
)


# ------------------------------------------------------------------
# Setup
add_library("Ray" SHARED "${SRC}")
target_include_directories("Ray"
	PUBLIC
		"${PUBLIC_INC}"
	PRIVATE
		"${PRIVATE_INC}"
)
target_link_libraries("Ray"
	PUBLIC
		"${PUBLIC_LIB}"
	PRIVATE
		"${PRIVATE_LIB}"
)
target_compile_definitions("Ray" PRIVATE "RAY_BUILD" "ENTT_DLL_EXPORT")
target_precompile_headers("Ray" PRIVATE "Rpch.h")

AddRecursiveFilters("${SRC}")
SetOutputDirectory("Ray" "")
SetWorkingDirectoryPlane("Ray" "${CMAKE_SOURCE_DIR}")


# ------------------------------------------------------------------
# Custom commands
if(UNIX)
add_custom_command(TARGET "Ray" PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/Resources/"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/Resources>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/Resources>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/Resources>"
)
endif()

if(WIN32)
add_custom_command(TARGET "Ray" POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/Extern/assimp/lib/x64/assimp-vc142-mt.dll"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo>"
)
endif()

