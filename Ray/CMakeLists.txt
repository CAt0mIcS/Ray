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
	"../Resources/Shaders/Flat_Text.vert"
	"../Resources/Shaders/Flat_Text.frag"
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
	"Core/RFrustum.h"
	"Core/RFrustum.cpp"
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
	# "ShaderGraph/Nodes/RNode.h"
	# "ShaderGraph/Nodes/RNode.cpp"
	# "ShaderGraph/Nodes/RCameraNode.h"
	# "ShaderGraph/Nodes/RCameraNode.cpp"
	# "ShaderGraph/Nodes/RFloatNode.h"
	# "ShaderGraph/Nodes/RFloatNode.cpp"
	# "ShaderGraph/Nodes/RMultiplyNode.h"
	# "ShaderGraph/Nodes/RMultiplyNode.cpp"
	# "ShaderGraph/Nodes/RSplitNode.h"
	# "ShaderGraph/Nodes/RSplitNode.cpp"
	# "ShaderGraph/Nodes/RTransformationMatrixNode.h"
	# "ShaderGraph/Nodes/RTransformationMatrixNode.cpp"
	# "ShaderGraph/Nodes/RVector4Node.h"
	# "ShaderGraph/Nodes/RVector4Node.cpp"
	# "ShaderGraph/Nodes/RVertexNode.h"
	# "ShaderGraph/Nodes/RVertexNode.cpp"
	# "ShaderGraph/Nodes/RVertexOutputNode.h"
	# "ShaderGraph/Nodes/RVertexOutputNode.cpp"
	# "ShaderGraph/Nodes/RInputNode.h"
	# "ShaderGraph/Nodes/RInputNode.cpp"
	# "ShaderGraph/Nodes/ROutputNode.h"
	# "ShaderGraph/Nodes/ROutputNode.cpp"
	# "ShaderGraph/Nodes/RTexture2DNode.h"
	# "ShaderGraph/Nodes/RTexture2DNode.cpp"
	# "ShaderGraph/Nodes/RSampler2DNode.h"
	# "ShaderGraph/Nodes/RSampler2DNode.cpp"
	# "ShaderGraph/Nodes/RPropertyNode.h"
	# "ShaderGraph/Nodes/RPropertyNode.cpp"
	# 
	# "ShaderGraph/RShaderGenerator.h"
	# "ShaderGraph/RShaderGenerator.cpp"

	# ------------------------------------------------------------------
	# Graphics
	"Graphics/RGraphics.h"
	"Graphics/RGraphics.cpp"
	"Graphics/RCodex.h"
	
	"Graphics/Text/RFont.h"
	"Graphics/Text/RFont.cpp"

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

	"Graphics/Pipelines/RPipeline.h"
	"Graphics/Pipelines/RPipeline.cpp"
	"Graphics/Pipelines/RGraphicsPipeline.h"
	"Graphics/Pipelines/RGraphicsPipeline.cpp"
	"Graphics/Pipelines/RComputePipeline.h"
	"Graphics/Pipelines/RComputePipeline.cpp"
	"Graphics/Pipelines/Shader/RGlslCompiler.h"
	"Graphics/Pipelines/Shader/RGlslCompiler.cpp"
	"Graphics/Pipelines/Shader/RShader.h"
	"Graphics/Pipelines/Shader/RShader.cpp"
	"Graphics/Pipelines/Shader/RShaderReflection.h"
	"Graphics/Pipelines/Shader/RShaderReflection.cpp"
	"Graphics/Pipelines/Shader/RShaderTypes.h"
	"Graphics/Pipelines/Shader/DataAccess/RDescriptor.h"
	"Graphics/Pipelines/Shader/DataAccess/RDescriptor.cpp"
	"Graphics/Pipelines/Shader/DataAccess/RBufferUniform.h"
	"Graphics/Pipelines/Shader/DataAccess/RBufferUniform.cpp"
	"Graphics/Pipelines/Shader/DataAccess/RSampler2DUniform.h"
	"Graphics/Pipelines/Shader/DataAccess/RSampler2DUniform.cpp"

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
	"Graphics/Buffers/Dynamic/RDynamicBuffer.h"
	"Graphics/Buffers/Dynamic/RDynamicBuffer.cpp"
	"Graphics/Buffers/Dynamic/RDynamicBufferAccess.h"
	"Graphics/Buffers/Dynamic/RDynamicBufferAccess.cpp"
	"Graphics/Buffers/Dynamic/RDynamicUniformBuffer.h"
	"Graphics/Buffers/Dynamic/RDynamicUniformBuffer.cpp"

	"Graphics/Images/RImage.h"
	"Graphics/Images/RImage.cpp"
	"Graphics/Images/RDepthImage.h"
	"Graphics/Images/RDepthImage.cpp"
	"Graphics/Images/RImageView.h"
	"Graphics/Images/RImageView.cpp"
	"Graphics/Images/RTexture.h"
	"Graphics/Images/RTexture.cpp"
	"Graphics/Images/RTextureCubemap.h"
	"Graphics/Images/RTextureCubemap.cpp"
	"Graphics/Images/RTextureSampler.h"
	"Graphics/Images/RTextureSampler.cpp"
	"Graphics/Images/RTexture2DAtlas.h"
	"Graphics/Images/RTexture2DAtlas.cpp"

	"Graphics/Commands/RCommandBuffer.h"
	"Graphics/Commands/RCommandBuffer.cpp"
	"Graphics/Commands/RCommandPool.h"
	"Graphics/Commands/RCommandPool.cpp"
	"Graphics/Commands/RCommandBufferRecorder.h"
	"Graphics/Commands/RCommandBufferRecorder.cpp"

	"Graphics/RenderPass/RRenderPass.h"
	"Graphics/RenderPass/RRenderPass.cpp"
	"Graphics/RenderPass/RAttachment.h"
	"Graphics/RenderPass/RAttachment.cpp"
	"Graphics/RenderPass/RSubpass.h"
	"Graphics/RenderPass/RSubpass.cpp"

	# ------------------------------------------------------------------
	# Components
	"Components/Base/RRenderer.h"
	"Components/Base/RRenderer.cpp"
	"Components/Base/RWidget.h"
	"Components/Base/RWidget.cpp"
	"Components/RComponent.h"
	"Components/RMesh.h"
	"Components/RMesh.cpp"
	"Components/RSkybox.h"
	"Components/RSkybox.cpp"
	"Components/RMeshRenderer.h"
	"Components/RMeshRenderer.cpp"
	"Components/RTransform.h"
	"Components/RTransform.cpp"
	"Components/RScriptableEntity.h"
	"Components/RScriptableEntity.cpp"
	"Components/RTagComponent.h"
	"Components/RTagComponent.cpp"
	"Components/RHierachyComponent.h"
	"Components/RHierachyComponent.cpp"

	# UI
	"Components/UI/RButton.h"
	"Components/UI/RButton.cpp"
	"Components/UI/RTextComponent.h"
	"Components/UI/RTextComponent.cpp"

	# Shading
	"Shading/RMaterial.h"
	"Shading/RMaterial.cpp"
	"Shading/RMaterialDataContainer.h"
	"Shading/RMaterialDataContainer.cpp"
	
	# ------------------------------------------------------------------
	# File system
	"Filesystem/RJSON.h"
	"Filesystem/RJSON.cpp"

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
	"Utils/RTypeTraits.h"
	"Utils/RImGui.h"
	"Utils/RImGui.cpp"

	# ------------------------------------------------------------------
	# Event system
	"Events/REngineEvents.h"
	"Events/RMouseEvents.h"
	"Events/RKeyboardEvents.h"
	"Events/RCustomEvents.h"
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
    "${CMAKE_SOURCE_DIR}/Extern/rapidjson/include"
    "${CMAKE_SOURCE_DIR}/Extern/glfw/include"
    "${CMAKE_SOURCE_DIR}/Extern"
    "${CMAKE_SOURCE_DIR}/Extern/freetype"
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
    "freetype"
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

