project "RlLogger"
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
        "RlUtilities"
    }

    libdirs
    {
        targetOutDir
    }

    links
    {
        "RlUtilities.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "RlL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        symbols "on"
        runtime "Debug"
        defines "RLL_DEBUG"

    filter "configurations:Release"
        optimize "speed"
        runtime "Release"
        defines "RLL_RELEASE"