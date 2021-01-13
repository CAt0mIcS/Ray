set(SRC
    "include/RayRender/RRBase.h"
    "include/RayRender/RRpch.h"
    "include/RayRender/RRendererAPI.h"

    # Core functionality classes
    "include/RayRender/Core/RScene.h"
    "include/RayRender/Core/RScene.cpp"

    # Render Primitives
    "include/RayRender/Primitives/RDrawable.h"
    "include/RayRender/Primitives/RDrawable.cpp"

    # Abstract classes
    "include/RayRender/RRenderer3D.h"
    "include/RayRender/RRenderer3D.cpp"
    "include/RayRender/RVertexBuffer.h"
    "include/RayRender/RVertexBuffer.cpp"

    # DirectX11 classes
    "include/RayRender/Platform/DX11/RDX11Renderer3D.h"
    "include/RayRender/Platform/DX11/RDX11Renderer3D.cpp"
    "include/RayRender/Platform/DX11/RDX11GraphicsResources.h"
    "include/RayRender/Platform/DX11/RDX11GraphicsResources.cpp"
    "include/RayRender/Platform/DX11/RDX11VertexBuffer.h"
    "include/RayRender/Platform/DX11/RDX11VertexBuffer.cpp"
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
# Copy pre-compiled assimp dll into output directory
add_custom_command(TARGET "RayRender" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/../extern/assimp/lib/${RL_ARCHITECTURE}/assimp-vc142-mt.dll"
    "$<$<CONFIG:DEBUG>:${CMAKE_BINARY_DIR}/bin/Debug/>"
    "$<$<CONFIG:RELEASE>:${CMAKE_BINARY_DIR}/bin/Release/>"
    "$<$<CONFIG:RELWITHDEBINFO>:${CMAKE_BINARY_DIR}/bin/RelWithDebInfo/>"
)


# Set Pixel and Vertex Shaders to compile in VS
# if(MSVC)
#     set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_TYPE Pixel)
#     set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
#     set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
#     set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
#     set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
#     
#     set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_TYPE Vertex)
#     set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
#     set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
#     set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
#     set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
#     set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SETTINGS "$<$<CONFIG:DEBUG>:SourceProperty1=SourceProperty1Value>")
# endif()
