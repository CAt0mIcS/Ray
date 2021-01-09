set(SRC
    "include/RayRender/RlRBase.h"
    "include/RayRender/rlrpch.h"
    
    "include/RayRender/Renderer3D.h"
    "include/RayRender/Renderer3D.cpp"
    "include/RayRender/GraphicsResource.cpp"
    "include/RayRender/GraphicsResource.h"
    "include/RayRender/RendererAPI.h"
    "include/RayRender/RendererAPI.cpp"
    "include/RayRender/Camera.h"
    "include/RayRender/Camera.cpp"
    "include/RayRender/PointLight.h"
    "include/RayRender/PointLight.cpp"

    # ----------------------------------------------
    # Bindables
    "include/RayRender/Bindable/Bindable.h"
    "include/RayRender/Bindable/Bindable.cpp"
    "include/RayRender/Bindable/VertexBuffer.h"
    "include/RayRender/Bindable/VertexBuffer.cpp"
    "include/RayRender/Bindable/IndexBuffer.h"
    "include/RayRender/Bindable/IndexBuffer.cpp"
    "include/RayRender/Bindable/Topology.h"
    "include/RayRender/Bindable/Topology.cpp"
    "include/RayRender/Bindable/VertexShader.h"
    "include/RayRender/Bindable/VertexShader.cpp"
    "include/RayRender/Bindable/PixelShader.h"
    "include/RayRender/Bindable/PixelShader.cpp"
    "include/RayRender/Bindable/InputLayout.h"
    "include/RayRender/Bindable/InputLayout.cpp"
    "include/RayRender/Bindable/ConstantBuffers.h"
    "include/RayRender/Bindable/ConstantBuffers.cpp"

    # ----------------------------------------------
    # Drawables
    "include/RayRender/Drawable/Drawable.h"
    "include/RayRender/Drawable/Drawable.cpp"
    "include/RayRender/Drawable/Triangle.h"
    "include/RayRender/Drawable/Triangle.cpp"
    "include/RayRender/Drawable/Cube.h"
    "include/RayRender/Drawable/Cube.cpp"
    "include/RayRender/Drawable/Model.h"
    "include/RayRender/Drawable/Model.cpp"
    "include/RayRender/Drawable/Mesh.h"
    "include/RayRender/Drawable/Mesh.cpp"
    "include/RayRender/Drawable/ShadedCube.h"
    "include/RayRender/Drawable/ShadedCube.cpp"
    "include/RayRender/Drawable/SolidSphere.h"
    "include/RayRender/Drawable/SolidSphere.cpp"
    "include/RayRender/Drawable/Rectangle.h"
    "include/RayRender/Drawable/Rectangle.cpp"


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
    "Shaders/PixelShader(Cube)-p.hlsl"
    "Shaders/VertexShader(Cube)-v.hlsl"
    "Shaders/PixelShader(ShadedCube)-p.hlsl"
    "Shaders/VertexShader(ShadedCube)-v.hlsl"
    "Shaders/SolidVS-v.hlsl"
    "Shaders/SolidPS-p.hlsl"
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

# add_dependencies("RayRender" "RlDebug")
target_compile_definitions("RayRender" PRIVATE "RR_BUILD")
target_precompile_headers("RayRender" PRIVATE "include/RayRender/rlRpch.h")

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
if(MSVC)
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_TYPE Pixel)
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "Shaders/PixelShader-p.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_TYPE Vertex)
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    set_property(SOURCE "Shaders/VertexShader-v.hlsl" PROPERTY VS_SETTINGS "$<$<CONFIG:DEBUG>:SourceProperty1=SourceProperty1Value>")


    set_property(SOURCE "Shaders/PixelShader(Cube)-p.hlsl" PROPERTY VS_SHADER_TYPE Pixel)
    set_property(SOURCE "Shaders/PixelShader(Cube)-p.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "Shaders/PixelShader(Cube)-p.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
    set_property(SOURCE "Shaders/PixelShader(Cube)-p.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "Shaders/PixelShader(Cube)-p.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    
    set_property(SOURCE "Shaders/VertexShader(Cube)-v.hlsl" PROPERTY VS_SHADER_TYPE Vertex)
    set_property(SOURCE "Shaders/VertexShader(Cube)-v.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "Shaders/VertexShader(Cube)-v.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
    set_property(SOURCE "Shaders/VertexShader(Cube)-v.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "Shaders/VertexShader(Cube)-v.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    set_property(SOURCE "Shaders/VertexShader(Cube)-v.hlsl" PROPERTY VS_SETTINGS "$<$<CONFIG:DEBUG>:SourceProperty1=SourceProperty1Value>")


    set_property(SOURCE "Shaders/PixelShader(ShadedCube)-p.hlsl" PROPERTY VS_SHADER_TYPE Pixel)
    set_property(SOURCE "Shaders/PixelShader(ShadedCube)-p.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "Shaders/PixelShader(ShadedCube)-p.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
    set_property(SOURCE "Shaders/PixelShader(ShadedCube)-p.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "Shaders/PixelShader(ShadedCube)-p.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    
    set_property(SOURCE "Shaders/VertexShader(ShadedCube)-v.hlsl" PROPERTY VS_SHADER_TYPE Vertex)
    set_property(SOURCE "Shaders/VertexShader(ShadedCube)-v.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "Shaders/VertexShader(ShadedCube)-v.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
    set_property(SOURCE "Shaders/VertexShader(ShadedCube)-v.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "Shaders/VertexShader(ShadedCube)-v.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    set_property(SOURCE "Shaders/VertexShader(ShadedCube)-v.hlsl" PROPERTY VS_SETTINGS "$<$<CONFIG:DEBUG>:SourceProperty1=SourceProperty1Value>")

    
    set_property(SOURCE "Shaders/SolidPS-p.hlsl" PROPERTY VS_SHADER_TYPE Pixel)
    set_property(SOURCE "Shaders/SolidPS-p.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "Shaders/SolidPS-p.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
    set_property(SOURCE "Shaders/SolidPS-p.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "Shaders/SolidPS-p.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    
    set_property(SOURCE "Shaders/SolidVS-v.hlsl" PROPERTY VS_SHADER_TYPE Vertex)
    set_property(SOURCE "Shaders/SolidVS-v.hlsl" PROPERTY VS_SHADER_ENTRYPOINT "main")
    set_property(SOURCE "Shaders/SolidVS-v.hlsl" PROPERTY VS_SHADER_MODEL 5.0)
    set_property(SOURCE "Shaders/SolidVS-v.hlsl" PROPERTY VS_SHADER_FLAGS "/DFLAGS_ADDED")
    set_property(SOURCE "Shaders/SolidVS-v.hlsl" PROPERTY VS_SHADER_OUTPUT_HEADER_FILE "$(OutDir)%(Filename).h")
    set_property(SOURCE "Shaders/SolidVS-v.hlsl" PROPERTY VS_SETTINGS "$<$<CONFIG:DEBUG>:SourceProperty1=SourceProperty1Value>")
endif()
