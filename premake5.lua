workspace "NodePlanningEditor"
    startproject "Zeal"
    
    configurations
    {
        "Debug",
        "Release"
    }

    platforms
    {
        "x86",
        "x86_64"
    }

    filter "platforms:x86"
        system "Windows"
        architecture "x86"

    filter "platforms:x86_64"
        system "Windows"
        architecture "x64"

    filter "files:**-v.hlsl"
        shaderobjectfileoutput("Shaders/%{file.basename}.cso")
        shadertype "Vertex"

    filter "files:**-p.hlsl"
        shaderobjectfileoutput("Shaders/%{file.basename}.cso")
        shadertype "Pixel"


outputDir = "%{cfg.buildcfg}-%{cfg.system}-$(Platform)"
targetOutDir = "bin/%{cfg.buildcfg}-%{cfg.system}-$(Platform)"
cwd = os.getcwd()


project "Reyal-GUI"
    targetname "Reyal"
    location "Reyal-GUI"
    kind "SharedLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "default"

    targetdir(targetOutDir)
    objdir("bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "rlpch.h"
    pchsource "Reyal-GUI/src/rlpch.cpp"

    dependson
    {
        "ZealLogger"
    }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/Shaders/**.hlsl"
    }

    includedirs
    {
        "%{prj.name}/src",
        "ZealLogger/src"
    }

    links
    {
        "ZealLogger.lib",
        "D2D1.lib",
        "D3D11.lib",
        "D3DCompiler.lib"
    }

    libdirs
    {
        targetOutDir
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "RL_PLATFORM_WINDOWS",
            "RL_BUILD"
        }

    filter "configurations:Debug"
        defines "RL_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RL_RELEASE"
        runtime "Release"
        optimize "speed"




project "Zeal"
    location "Zeal"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "default"

    targetdir("bin/" .. outputDir)
    objdir("bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "Zeal/src/pch.cpp"

    dependson
    {
        "Reyal-GUI",
        "ZealLogger",
        "ZealGUILayer"
    }

    defines
    {
        "ZL_OUT_DIR=\"".. cwd.. "/".. targetOutDir .. "\""
    }

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src",
        "ZealLogger/src",
        "Reyal-GUI/src"
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


project "ZealLogger"
    location "ZealLogger"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "default"

    targetdir("bin/" .. outputDir)
    objdir("bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "zlpch.h"
    pchsource "ZealLogger/src/zlpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/src"
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

group "ExtensionLayers"
    project "ZealGUILayer"
        kind "SharedLib"
        language "C++"
        cppdialect "C++17"
        location "ZealGUILayer"
        staticruntime "default"

        targetdir("bin/" .. outputDir .. "/Editors")
        objdir("bin-int/" .. outputDir .. "/%{prj.name}")

        pchheader "glpch.h"
        pchsource "ZealGUILayer/src/glpch.cpp"

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
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }

        includedirs
        {
            "%{prj.name}/src",
            "Reyal-GUI/src",
            "ZealLogger/src"
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
group "" -- End of group "ExtensionLayers"