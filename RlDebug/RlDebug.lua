project "RlDebug"
    language "C++"
    cppdialect "C++17"
    kind "SharedLib"

    targetdir(targetOutDir)
    objdir("../bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "rldpch.h"
    pchsource "src/rldpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "**.def"
    }

    includedirs
    {
        "src/",
        "../RlUtilities/src",
        "../RlLogger/src"
    }

    dependson
    {
        "RlLogger"
    }

    libdirs
    {
        targetOutDir
    }

    links
    {
        "RlUtilities.lib"
    }

    defines
    {
        "RLD_BUILD"
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