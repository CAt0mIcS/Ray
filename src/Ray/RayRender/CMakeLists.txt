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
    "include/RayRender/Core/RPerspectiveCamera.h"
    "include/RayRender/Core/RPerspectiveCamera.cpp"

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
    "include/RayRender/RVertexShader.h"
    "include/RayRender/RVertexShader.cpp"
    "include/RayRender/RPixelShader.h"
    "include/RayRender/RPixelShader.cpp"
    "include/RayRender/RIndexBuffer.h"
    "include/RayRender/RIndexBuffer.cpp"
    "include/RayRender/RTopology.h"
    "include/RayRender/RTopology.cpp"
    "include/RayRender/RConstantBuffers.h"
    "include/RayRender/RConstantBuffers.cpp"

    # DirectX11 classes
    "include/RayRender/Platform/DX11/RDX11Renderer3D.h"
    "include/RayRender/Platform/DX11/RDX11Renderer3D.cpp"
    "include/RayRender/Platform/DX11/RDX11GraphicsResources.h"
    "include/RayRender/Platform/DX11/RDX11GraphicsResources.cpp"
    "include/RayRender/Platform/DX11/RDX11VertexBuffer.h"
    "include/RayRender/Platform/DX11/RDX11VertexBuffer.cpp"
    "include/RayRender/Platform/DX11/RDX11VertexShader.h"
    "include/RayRender/Platform/DX11/RDX11VertexShader.cpp"
    "include/RayRender/Platform/DX11/RDX11PixelShader.h"
    "include/RayRender/Platform/DX11/RDX11PixelShader.cpp"
    "include/RayRender/Platform/DX11/RDX11IndexBuffer.h"
    "include/RayRender/Platform/DX11/RDX11IndexBuffer.cpp"
    "include/RayRender/Platform/DX11/RDX11Topology.h"
    "include/RayRender/Platform/DX11/RDX11Topology.cpp"
    "include/RayRender/Platform/DX11/RDX11ConstantBuffers.h"
    "include/RayRender/Platform/DX11/RDX11ConstantBuffers.cpp"

    # OpenGL classes
    "include/RayRender/Platform/OpenGL/ROpenGLRenderer3D.h"
    "include/RayRender/Platform/OpenGL/ROpenGLRenderer3D.cpp"
    "include/RayRender/Platform/OpenGL/ROpenGLVertexBuffer.h"
    "include/RayRender/Platform/OpenGL/ROpenGLVertexBuffer.cpp"
    "include/RayRender/Platform/OpenGL/ROpenGLVertexShader.h"
    "include/RayRender/Platform/OpenGL/ROpenGLVertexShader.cpp"
    "include/RayRender/Platform/OpenGL/ROpenGLPixelShader.h"
    "include/RayRender/Platform/OpenGL/ROpenGLPixelShader.cpp"
    "include/RayRender/Platform/OpenGL/ROpenGLIndexBuffer.h"
    "include/RayRender/Platform/OpenGL/ROpenGLIndexBuffer.cpp"
    "include/RayRender/Platform/OpenGL/ROpenGLTopology.h"
    "include/RayRender/Platform/OpenGL/ROpenGLTopology.cpp"
    "include/RayRender/Platform/OpenGL/ROpenGLShaderBase.h"
    "include/RayRender/Platform/OpenGL/ROpenGLShaderBase.cpp"
    "include/RayRender/Platform/OpenGL/ROpenGLConstantBuffers.h"
    "include/RayRender/Platform/OpenGL/ROpenGLConstantBuffers.cpp"

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

set(PUBLIC_INC
    "include"
)

set(PRIVATE_INC
    ""
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
