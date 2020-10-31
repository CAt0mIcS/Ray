project "ZlUtilities"
    language "C++"
    cppdialect "C++17"
    kind "StaticLib"

    targetdir(targetOutDir)
    objdir("../bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "utpch.h"
    pchsource "src/utpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "UT_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        symbols "on"
        runtime "Debug"
        defines "UT_DEBUG"

    filter "configurations:Release"
        optimize "speed"
        runtime "Release"
        defines "UT_RELEASE"