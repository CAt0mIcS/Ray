set(SRC
    "include/RayRender/RayRender.h"
    "include/RayRender/RRBase.h"
    "include/RayRender/RRpch.h"
    "include/RayRender/RRendererAPI.h"
    "include/RayRender/RRendererAPI.cpp"

    # Core functionality classes
    "include/RayRender/Core/RVertex.h"
    "include/RayRender/Core/RScene.h"
    "include/RayRender/Core/RScene.cpp"
    "include/RayRender/Core/REntity.h"
    "include/RayRender/Core/REntity.cpp"
    "include/RayRender/Core/RCamera.h"
    "include/RayRender/Core/RCamera.cpp"
    "include/RayRender/Core/RPerspectiveCamera.h"
    "include/RayRender/Core/RPerspectiveCamera.cpp"
    "include/RayRender/Core/ROrthographicCamera.h"
    "include/RayRender/Core/ROrthographicCamera.cpp"

    # Render Primitives
    "include/RayRender/Primitives/RDrawable.h"
    "include/RayRender/Primitives/RDrawable.cpp"
    "include/RayRender/Primitives/RQuad.h"
    "include/RayRender/Primitives/RQuad.cpp"
    "include/RayRender/Primitives/RTriangle.h"
    "include/RayRender/Primitives/RTriangle.cpp"

    # Abstract classes
    "include/RayRender/RRenderer3D.h"
    "include/RayRender/RRenderer3D.cpp"
    "include/RayRender/RVertexBuffer.h"
    "include/RayRender/RVertexBuffer.cpp"
    "include/RayRender/RShader.h"
    "include/RayRender/RShader.cpp"
    "include/RayRender/RIndexBuffer.h"
    "include/RayRender/RIndexBuffer.cpp"
    "include/RayRender/RTopology.h"
    "include/RayRender/RTopology.cpp"
    "include/RayRender/RConstantBuffers.h"
    "include/RayRender/RConstantBuffers.cpp"

    # DirectX11 classes
    "include/RayRender/Platform/DX11/RDX11Renderer3D.h"
    "include/RayRender/Platform/DX11/RDX11Renderer3D.cpp"
    "include/RayRender/Platform/DX11/RDX11VertexBuffer.h"
    "include/RayRender/Platform/DX11/RDX11VertexBuffer.cpp"
    "include/RayRender/Platform/DX11/RDX11Shader.h"
    "include/RayRender/Platform/DX11/RDX11Shader.cpp"
    "include/RayRender/Platform/DX11/RDX11IndexBuffer.h"
    "include/RayRender/Platform/DX11/RDX11IndexBuffer.cpp"
    "include/RayRender/Platform/DX11/RDX11Topology.h"
    "include/RayRender/Platform/DX11/RDX11Topology.cpp"
    "include/RayRender/Platform/DX11/RDX11ConstantBuffers.h"
    "include/RayRender/Platform/DX11/RDX11ConstantBuffers.cpp"
    "include/RayRender/Platform/DX11/Internal/RDX11GraphicsResources.h"
    "include/RayRender/Platform/DX11/Internal/RDX11GraphicsResources.cpp"

    # OpenGL classes
    "include/RayRender/Platform/OpenGL/ROpenGLRenderer3D.h"
    "include/RayRender/Platform/OpenGL/ROpenGLRenderer3D.cpp"
    "include/RayRender/Platform/OpenGL/ROpenGLVertexBuffer.h"
    "include/RayRender/Platform/OpenGL/ROpenGLVertexBuffer.cpp"
    "include/RayRender/Platform/OpenGL/ROpenGLShader.h"
    "include/RayRender/Platform/OpenGL/ROpenGLShader.cpp"
    "include/RayRender/Platform/OpenGL/ROpenGLIndexBuffer.h"
    "include/RayRender/Platform/OpenGL/ROpenGLIndexBuffer.cpp"
    "include/RayRender/Platform/OpenGL/ROpenGLTopology.h"
    "include/RayRender/Platform/OpenGL/ROpenGLTopology.cpp"
    "include/RayRender/Platform/OpenGL/ROpenGLConstantBuffers.h"
    "include/RayRender/Platform/OpenGL/ROpenGLConstantBuffers.cpp"

    # Vulkan classes
    "include/RayRender/Platform/Vulkan/RVulkanRenderer3D.h"
    "include/RayRender/Platform/Vulkan/RVulkanRenderer3D.cpp"
    "include/RayRender/Platform/Vulkan/RVulkanVertexBuffer.h"
    "include/RayRender/Platform/Vulkan/RVulkanVertexBuffer.cpp"
    "include/RayRender/Platform/Vulkan/RVulkanShader.h"
    "include/RayRender/Platform/Vulkan/RVulkanShader.cpp"
    "include/RayRender/Platform/Vulkan/RVulkanIndexBuffer.h"
    "include/RayRender/Platform/Vulkan/RVulkanIndexBuffer.cpp"
    "include/RayRender/Platform/Vulkan/RVulkanTopology.h"
    "include/RayRender/Platform/Vulkan/RVulkanTopology.cpp"
    "include/RayRender/Platform/Vulkan/RVulkanConstantBuffers.h"
    "include/RayRender/Platform/Vulkan/RVulkanConstantBuffers.cpp"
    "include/RayRender/Platform/Vulkan/Internal/RVulkanDevice.h"
    "include/RayRender/Platform/Vulkan/Internal/RVulkanDevice.cpp"

    # DirectX11 Shaders
    "shaders/QuadVertexShader.hlsl"
    "shaders/QuadPixelShader.hlsl"
    "shaders/TriangleVertexShader.hlsl"
    "shaders/TrianglePixelShader.hlsl"

    # OpenGL Shaders
    # "shaders/QuadVertexShader.glsl"
    # "shaders/QuadPixelShader.glsl"
    # "shaders/TriangleVertexShader.glsl"
    # "shaders/TrianglePixelShader.glsl"
)

find_package("Vulkan" REQUIRED FATAL_ERROR)

set(PUBLIC_INC
    "include"
)

set(PRIVATE_INC
    "${Vulkan_INCLUDE_DIRS}"
)

set(PUBLIC_LIB
	""
)

set(PRIVATE_LIB
    "GLFW"
    "Glad"
    "RayDebug"
    "RayEvent"
    "assimp"
    "ShaderConductor"
    "${Vulkan_LIBRARIES}"
)

if(WIN32)
list(APPEND PRIVATE_LIB "D3D11" "DXGI" "D3DCompiler")
endif()



add_library("RayRender" SHARED "${SRC}")

target_include_directories("RayRender"
    PRIVATE
        "${PRIVATE_INC}"
    PUBLIC
        "${PUBLIC_INC}"
)
target_link_libraries("RayRender"
    PRIVATE
        "${PRIVATE_LIB}"
    PUBLIC
        "${PUBLIC_LIB}"
)

target_compile_definitions("RayRender" PRIVATE "RR_BUILD")
target_precompile_headers("RayRender" PRIVATE "include/RayRender/RRpch.h")

AddRecursiveFilters("${SRC}")
SetOutputDirectory("RayRender" "")
SetWorkingDirectory("RayRender" "")

# ------------------------------------------------------------------------------------------------
# External dependencies
add_subdirectory("extern")

# ------------------------------------------------------------------------------------------------
# Copy pre-compiled assimp dll into output directory
# set(RAY_OUT_DIR
#     "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/>"
#     "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/>"
#     "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/>"
# )

if(WIN32)
add_custom_command(TARGET "RayRender" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/assimp/lib/${RL_ARCHITECTURE}/assimp-vc142-mt.dll"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/>"
)
add_custom_command(TARGET "RayRender" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/ShaderConductor/lib/${RL_ARCHITECTURE}/ShaderConductor.dll"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/>"
)
add_custom_command(TARGET "RayRender" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/ShaderConductor/lib/${RL_ARCHITECTURE}/dxcompiler.dll"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/>"
)
elseif(UNIX)
add_custom_command(TARGET "RayRender" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/ShaderConductor/lib/${RL_ARCHITECTURE}/libShaderConductor.so"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/>"
)
add_custom_command(TARGET "RayRender" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/extern/ShaderConductor/lib/${RL_ARCHITECTURE}/libdxcompiler.so"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/>"

)
endif()

# ------------------------------------------------------------------------------------------------
# Copy HLSL shaders to the output directory
add_custom_command(TARGET "RayRender" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/QuadVertexShader.hlsl"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/>"
)
add_custom_command(TARGET "RayRender" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/QuadPixelShader.hlsl"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/>"
)
add_custom_command(TARGET "RayRender" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/TriangleVertexShader.hlsl"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/>"
)
add_custom_command(TARGET "RayRender" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/TrianglePixelShader.hlsl"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/>"
)


# Set Pixel and Vertex Shaders to compile in VS
if(MSVC)
    set_property(SOURCE "shaders/QuadPixelShader.hlsl" PROPERTY VS_SHADER_TYPE Pixel)
    set_property(SOURCE "shaders/QuadPixelShader.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "shaders/QuadPixelShader.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
    set_property(SOURCE "shaders/QuadPixelShader.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "shaders/QuadPixelShader.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    
    set_property(SOURCE "shaders/QuadVertexShader.hlsl" PROPERTY VS_SHADER_TYPE Vertex)
    set_property(SOURCE "shaders/QuadVertexShader.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "shaders/QuadVertexShader.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
    set_property(SOURCE "shaders/QuadVertexShader.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "shaders/QuadVertexShader.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    set_property(SOURCE "shaders/QuadVertexShader.hlsl" PROPERTY VS_SETTINGS "$<$<CONFIG:DEBUG>:SourceProperty1=SourceProperty1Value>")
    
    set_property(SOURCE "shaders/TrianglePixelShader.hlsl" PROPERTY VS_SHADER_TYPE Pixel)
    set_property(SOURCE "shaders/TrianglePixelShader.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "shaders/TrianglePixelShader.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
    set_property(SOURCE "shaders/TrianglePixelShader.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "shaders/TrianglePixelShader.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    
    set_property(SOURCE "shaders/TriangleVertexShader.hlsl" PROPERTY VS_SHADER_TYPE Vertex)
    set_property(SOURCE "shaders/TriangleVertexShader.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "shaders/TriangleVertexShader.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
    set_property(SOURCE "shaders/TriangleVertexShader.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "shaders/TriangleVertexShader.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    set_property(SOURCE "shaders/TriangleVertexShader.hlsl" PROPERTY VS_SETTINGS "$<$<CONFIG:DEBUG>:SourceProperty1=SourceProperty1Value>")
endif()
