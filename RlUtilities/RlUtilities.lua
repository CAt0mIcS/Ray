project "RlUtilities"
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