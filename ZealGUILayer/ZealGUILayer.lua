project "ZealGUILayer"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"

    targetdir(targetOutDir .. "/Editors")
    objdir("../bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "glpch.h"
    pchsource "src/glpch.cpp"

    dependson
    {
        "Reyal-GUI"
    }

    defines
    {
        "ZEAL_GUI_LAYER_BUILD"
    }

    files
    {
        "src/**.h",
        "src/**.cpp"
	}
	
    includedirs
    {
        "src",
        "../Reyal-GUI/src",
        "../ZealLogger/src",
        "../ZlUtilities/src"
    }

    libdirs
    {
        targetOutDir
    }

    links
    {
        "Reyal.lib",
        "ZealLogger.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "ZGL_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        symbols "on"
        runtime "Debug"
        defines "ZGL_DEBUG"

    filter "configurations:Release"
        optimize "speed"
        runtime "Release"
        defines "ZGL_RELEASE"