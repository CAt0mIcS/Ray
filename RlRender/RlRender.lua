project "RlRender"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"

    targetdir(targetOutDir)
    objdir("../bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "rlrpch.h"
    pchsource "src/rlrpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "../RlDebug/src"
    }

    links
    {
        "RlDebug.lib",
        "RlUtilities.lib",
        "DXGI.lib",
        "D3D11.lib",
        "D3DCompiler.lib"
    }

    dependson
    {
        "RlDebug"
    }

    libdirs
    {
        targetOutDir
    }

    defines
    {
        "RLR_BUILD"
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