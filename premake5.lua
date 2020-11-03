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
targetOutDir = "../bin/%{cfg.buildcfg}-%{cfg.system}-$(Platform)"
cwd = os.getcwd()

include "Reyal/Reyal.lua"
include "Zeal/Zeal.lua"
include "RlRender/RlRender.lua"


group "ExtensionLayers"
    include "ZealGUILayer/ZealGUILayer.lua"
group "" -- End of group "ExtensionLayers"

group "RlUtilities"
    include "RlLogger/RlLogger.lua"
    include "RlUtilities/RlUtilities.lua"
    include "RlDebug/RlDebug.lua"
group "" -- End of group RlUtilities
