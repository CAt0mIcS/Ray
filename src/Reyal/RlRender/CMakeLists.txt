set(SRC
    "include/RlRender/TypeDefines.h"
    "include/RlRender/RlRBase.h"
    "include/RlRender/rlrpch.h"
    "include/RlRender/Renderer3D.h"
    "include/RlRender/Renderer3D.cpp"

    "include/RlRender/GraphicsResource.cpp"
    "include/RlRender/GraphicsResource.h"
    "include/RlRender/RendererAPI.h"
    "include/RlRender/RendererAPI.cpp"

    # ----------------------------------------------
    # Rendering Interfaces

    # ----------------------------------------------
    # OpenGL

    # ----------------------------------------------
    # DirectX

    # ----------------------------------------------
    # Shaders
    "Shaders/PixelShader-p.hlsl"
    "Shaders/VertexShader-v.hlsl"
)

set(PUBLIC_INC
    "include"
)

set(PRIVATE_INC
    "include/RlRender"
    "${CMAKE_CURRENT_SOURCE_DIR}/../extern/glfw/include"
)

set(PUBLIC_LIB
	""
)

if(WIN32)
    set(PRIVATE_LIB
        "D3D11"
        "DXGI"
        "D3DCompiler"
        "GLFW"
	"Glad"
	"RlDebug"
    )
elseif(UNIX)
    set(PRIVATE_LIB
        "GLFW"
	"Glad"
	"RlDebug"
    )
endif()


add_library("RlRender" SHARED "${SRC}")

target_include_directories("RlRender"
    PRIVATE
        "${PRIVATE_INC}"
    PUBLIC
        "${PUBLIC_INC}"
)
target_link_libraries("RlRender"
    PRIVATE
        "${PRIVATE_LIB}"
    PUBLIC
        "${PUBLIC_LIB}"
)

# add_dependencies("RlRender" "RlDebug")
target_compile_definitions("RlRender" PRIVATE "RLR_BUILD")
target_precompile_headers("RlRender" PRIVATE "include/RlRender/rlrpch.h")

# Set Pixel and Vertex Shaders to compile in VS
if(MSVC)
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_TYPE Pixel)
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_MODEL 4.0_level_9_3)
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_TYPE Vertex)
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_MODEL 4.0_level_9_3)
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SETTINGS "$<$<CONFIG:DEBUG>:SourceProperty1=SourceProperty1Value>")
endif()

AddRecursiveFilters("${SRC}")
SetOutputDirectory("RlRender" "")
