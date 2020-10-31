project "Reyal-GUI"
    targetname "Reyal"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"

    targetdir(targetOutDir)
    objdir("../bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "rlpch.h"
    pchsource "src/rlpch.cpp"

    dependson
    {
        "ZealLogger"
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
        "../ZealLogger/src",
        "../ZlUtilities/src"
    }

    links
    {
        "ZealLogger.lib",
        "D2D1.lib",
        "D3D11.lib",
        "D3DCompiler.lib",
        "DXGI.lib"
    }

    libdirs
    {
        targetOutDir
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "RL_PLATFORM_WINDOWS",
            "RL_BUILD"
        }

    filter "configurations:Debug"
        defines "RL_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RL_RELEASE"
        runtime "Release"
        optimize "speed"