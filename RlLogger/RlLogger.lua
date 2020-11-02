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