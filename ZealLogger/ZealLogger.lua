project "ZealLogger"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir(targetOutDir)
    objdir("../bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "zlpch.h"
    pchsource "src/zlpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src"
    }

    dependson
    {
        "ZlUtilities"
    }

    libdirs
    {
        targetOutDir
    }

    links
    {
        "ZlUtilities.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "ZLL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        symbols "on"
        runtime "Debug"
        defines "ZLL_DEBUG"

    filter "configurations:Release"
        optimize "speed"
        runtime "Release"
        defines "ZLL_RELEASE"