set(SHADERS
	"../Resources/Shaders/Flat_Col.vert"
	"../Resources/Shaders/Flat_Col.frag"
	"../Resources/Shaders/Flat_Diff.vert"
	"../Resources/Shaders/Flat_Diff.frag"
	"../Resources/Shaders/Phong_Col.vert"
	"../Resources/Shaders/Phong_Col.frag"
	"../Resources/Shaders/Phong_Spec.vert"
	"../Resources/Shaders/Phong_Spec.frag"
	"../Resources/Shaders/Phong_Diff.vert"
	"../Resources/Shaders/Phong_Diff.frag"
	"../Resources/Shaders/Phong_Diff_Norm.vert"
	"../Resources/Shaders/Phong_Diff_Norm.frag"
	"../Resources/Shaders/Phong_Diff_Spec.vert"
	"../Resources/Shaders/Phong_Diff_Spec.frag"
	"../Resources/Shaders/UI.vert"
	"../Resources/Shaders/UI.frag"
	"../Resources/Shaders/UI_Tex.vert"
	"../Resources/Shaders/UI_Tex.frag"
	"../Resources/Shaders/ImGui.vert"
	"../Resources/Shaders/ImGui.frag"
	"../Resources/Shaders/Skybox.vert"
	"../Resources/Shaders/Skybox.frag"
)

set(SRC
	"Rpch.h"
	"RBase.h"

	# ------------------------------------------------------------------
	# Scene
	"Scene/REntity.h"
	"Scene/REntity.cpp"
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
	"Core/RDynamicVertex.h"
	"Core/RDynamicVertex.cpp"
	"Core/RResource.h"
	"Core/RResource.cpp"
	
	# ------------------------------------------------------------------
	# Devices
	"Devices/RWindow.h"
	"Devices/RWindow.cpp"
	"Devices/RMouse.h"
	"Devices/RMouse.cpp"
	"Devices/RKeyboard.h"
	"Devices/RKeyboard.cpp"

	# ------------------------------------------------------------------
	# Shader graph
	"ShaderGraph/Nodes/RNode.h"
	"ShaderGraph/Nodes/RNode.cpp"
	"ShaderGraph/Nodes/RCameraNode.h"
	"ShaderGraph/Nodes/RCameraNode.cpp"
	"ShaderGraph/Nodes/RFloatNode.h"
	"ShaderGraph/Nodes/RFloatNode.cpp"
	"ShaderGraph/Nodes/RMultiplyNode.h"
	"ShaderGraph/Nodes/RMultiplyNode.cpp"
	"ShaderGraph/Nodes/RSplitNode.h"
	"ShaderGraph/Nodes/RSplitNode.cpp"
	"ShaderGraph/Nodes/RTransformationMatrixNode.h"
	"ShaderGraph/Nodes/RTransformationMatrixNode.cpp"
	"ShaderGraph/Nodes/RVector4Node.h"
	"ShaderGraph/Nodes/RVector4Node.cpp"
	"ShaderGraph/Nodes/RVertexNode.h"
	"ShaderGraph/Nodes/RVertexNode.cpp"
	"ShaderGraph/Nodes/RVertexOutputNode.h"
	"ShaderGraph/Nodes/RVertexOutputNode.cpp"
	"ShaderGraph/Nodes/RInputNode.h"
	"ShaderGraph/Nodes/RInputNode.cpp"
	"ShaderGraph/Nodes/ROutputNode.h"
	"ShaderGraph/Nodes/ROutputNode.cpp"
	"ShaderGraph/Nodes/RTexture2DNode.h"
	"ShaderGraph/Nodes/RTexture2DNode.cpp"
	"ShaderGraph/Nodes/RSampler2DNode.h"
	"ShaderGraph/Nodes/RSampler2DNode.cpp"
	"ShaderGraph/Nodes/RPropertyNode.h"
	"ShaderGraph/Nodes/RPropertyNode.cpp"

	"ShaderGraph/RShaderGenerator.h"
	"ShaderGraph/RShaderGenerator.cpp"

	# ------------------------------------------------------------------
	# Graphics
	"Graphics/RGraphics.h"
	"Graphics/RGraphics.cpp"
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
	"Graphics/Core/RSharedBindable.h"
	"Graphics/Core/RSharedBindable.cpp"

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
	"Graphics/Pipelines/Uniforms/RBufferUniform.h"
	"Graphics/Pipelines/Uniforms/RBufferUniform.cpp"
	"Graphics/Pipelines/Uniforms/RSampler2DUniform.h"
	"Graphics/Pipelines/Uniforms/RSampler2DUniform.cpp"

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
	"Graphics/Images/RTexture2D.h"
	"Graphics/Images/RTexture2D.cpp"
	"Graphics/Images/RTextureCubemap.h"
	"Graphics/Images/RTextureCubemap.cpp"
	"Graphics/Images/RTextureSampler.h"
	"Graphics/Images/RTextureSampler.cpp"

	"Graphics/Commands/RCommandBuffer.h"
	"Graphics/Commands/RCommandBuffer.cpp"
	"Graphics/Commands/RCommandPool.h"
	"Graphics/Commands/RCommandPool.cpp"

	"Graphics/RenderPass/RRenderPass.h"
	"Graphics/RenderPass/RRenderPass.cpp"
	"Graphics/RenderPass/RAttachment.h"
	"Graphics/RenderPass/RAttachment.cpp"
	"Graphics/RenderPass/RSubpass.h"
	"Graphics/RenderPass/RSubpass.cpp"

	# ------------------------------------------------------------------
	# Components
	"Components/RComponent.h"
	"Components/RMesh.h"
	"Components/RMesh.cpp"
	"Components/RSkybox.h"
	"Components/RSkybox.cpp"
	"Components/RMeshRenderer.h"
	"Components/RMeshRenderer.cpp"
	"Components/RTransform.h"
	"Components/RTransform.cpp"
	"Components/RParentEntity.h"
	"Components/RParentEntity.cpp"
	"Components/RScriptableEntity.h"
	"Components/RScriptableEntity.cpp"
	"Components/RTagComponent.h"
	"Components/RTagComponent.cpp"

	# Shading
	"Shading/RMaterial.h"
	"Shading/RMaterial.cpp"
	"Shading/Phong/RPhongMaterial.h"
	"Shading/Phong/RPhongMaterial.cpp"
	"Shading/Flat/RFlatColorMaterial.h"
	"Shading/Flat/RFlatColorMaterial.cpp"
	"Shading/Flat/RFlatTextureMaterial.h"
	"Shading/Flat/RFlatTextureMaterial.cpp"

	# ------------------------------------------------------------------
	# User Interface
	"UI/RWidget.h"
	"UI/RWidget.cpp"
	"UI/RButton.h"
	"UI/RButton.cpp"
	"UI/RImGui.h"
	"UI/RImGui.cpp"
	
	# ------------------------------------------------------------------
	# Mono
	"Mono/RScript.h"
	"Mono/RScript.cpp"
	"Mono/RObject.h"
	"Mono/RObject.cpp"
	"Mono/RFunctionBase.h"
	"Mono/RFunctionBase.cpp"
	"Mono/RFunction.h"
	"Mono/RFunction.cpp"
	"Mono/RStaticFunction.h"
	"Mono/RStaticFunction.cpp"

	# ------------------------------------------------------------------
	# Utilities
	"Utils/RString.h"
	"Utils/RString.cpp"
	"Utils/RThreadPool.h"
	"Utils/RThreadPool.cpp"
	"Utils/RNonCopyable.h"
	"Utils/RLogger.h"
	"Utils/RLogger.cpp"
	"Utils/RAssert.h"
	"Utils/RException.h"
	"Utils/RException.cpp"
	"Utils/RGeometricPrimitives.h"
	"Utils/RGeometricPrimitives.cpp"
	"Utils/RModel.h"
	"Utils/RModel.cpp"

	# ------------------------------------------------------------------
	# Event system
	"Events/REngineEvents.h"
	"Events/RMouseEvents.h"
	"Events/RKeyboardEvents.h"
	"Events/REventDispatcher.h"
	"Events/REventListener.h"

	"${SHADERS}"

	"../Extern/imgui/imgui.cpp"
	"../Extern/imgui/imgui_demo.cpp"
	"../Extern/imgui/imgui_draw.cpp"
	"../Extern/imgui/imgui_widgets.cpp"
	"../Extern/imgui/imgui_tables.cpp"
	"../Extern/imgui/imgui_impl_glfw.cpp"
	"../Extern/imgui/imgui_impl_vulkan.cpp"
)

# ------------------------------------------------------------------
# Find required packages
find_package("Vulkan" REQUIRED FATAL_ERROR)

# Get paths to Mono installation
SetMonoIncludesAndLibraries()

set(PUBLIC_INC
    "${Vulkan_INCLUDE_DIRS}"
)

set(PRIVATE_INC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_SOURCE_DIR}/Extern/entt/src"
    "${CMAKE_SOURCE_DIR}/Extern/glfw/include"
    "${CMAKE_SOURCE_DIR}/Extern"
    "${CMAKE_SOURCE_DIR}/Extern/Ion"
    "${MONO_INC}"
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
    # "ktx"
    "${MONO_LIB}"
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


# ------------------------------------------------------------------
# Copy dlls
if(WIN32)
add_custom_command(TARGET "Ray" POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/Extern/assimp/lib/x64/assimp-vc142-mt.dll"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo>"
)
add_custom_command(TARGET "Ray" PRE_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	"C:/Program FIles/Mono/bin/mono-2.0-sgen.dll"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo>"
)
endif()

