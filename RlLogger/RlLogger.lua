project "RlLogger"
    kind "None"
    language "C++"
    cppdialect "C++17"

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

    defines
    {
        "RLL_BUILD"
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