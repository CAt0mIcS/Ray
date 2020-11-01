project "Zeal"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"

    targetdir(targetOutDir)
    objdir("../bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    dependson
    {
        "Reyal-GUI",
        "ZealLogger",
        "ZealGUILayer"
    }

    defines
    {
        "ZL_OUT_DIR=\"".. cwd.. "/".. "bin/%{cfg.buildcfg}-%{cfg.system}-$(Platform)" .. "\""
    }

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src",
        "../ZealLogger/src",
        "../Reyal-GUI/src"
    }

    links
    {
        "ZealLogger.lib",
        "Reyal.lib"
    }

    libdirs
    {
        targetOutDir
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "ZL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        symbols "on"
        runtime "Debug"
        defines "ZL_DEBUG"

    filter "configurations:Release"
        optimize "speed"
        runtime "Release"
        defines "ZL_RELEASE"