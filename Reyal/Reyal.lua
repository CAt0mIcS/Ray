project "Reyal"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"

    targetdir(targetOutDir)
    objdir("../bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "rlpch.h"
    pchsource "src/rlpch.cpp"

    dependson
    {
        "RlLogger",
        "RlDebug"
    }

    files
    {
        "src/**.h",
        "src/**.cpp",
        "Shaders/**.hlsl"
    }

    includedirs
    {
        "src",
        "../RlLogger/src",
        "../RlDebug/src",
        "../RlUtilities/src"
    }

    links
    {
        "RlDebug.lib",
        "RlUtilities.lib",
        "D2D1.lib",
        "D3D11.lib",
        "D3DCompiler.lib",
        "DXGI.lib"
    }

    libdirs
    {
        targetOutDir
    }

    defines
    {
        "RL_BUILD"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "ZL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "ZL_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "ZL_RELEASE"
        runtime "Release"
        optimize "speed"