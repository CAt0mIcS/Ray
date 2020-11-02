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
        "Reyal"
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
        "../Reyal/src",
        "../RlDebug/src",
    }

    libdirs
    {
        targetOutDir
    }

    links
    {
        "Reyal.lib",
        "RlDebug.lib"
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