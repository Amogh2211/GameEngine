workspace "GameEngine"
    architecture "x64"
    startproject "Sandbox"
    
    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "GameEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "GameEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "GameEngine/vendor/imgui"
IncludeDir["glm"] = "GameEngine/vendor/GLM"


include "GameEngine/vendor/GLFW"
include "GameEngine/vendor/Glad"
include "GameEngine/vendor/imgui"



project "GameEngine"
    location "GameEngine"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


    pchheader "GameEnginepch.h"
	pchsource "GameEngine/src/GameEnginepch.cpp"


    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp", 
        "%{prj.name}/vendor/**.hpp",
        "%{prj.name}/vendor/**.inl"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
    }

    links
    {
        "GLFW",
        "Glad",
        "imgui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++14"
        staticruntime "Off"
        systemversion "10.0.22000.0"

        defines{
            "GE_PLATFORM_WINDOWS",
            "GE_BUILD_DLL",
            "_WINDLL",
            "GE_ENABLE_ASSERTS",
            "GLFW_INCLUDE_NONE"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "GE_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "GE_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "GE_DIST"
        buildoptions "/MD"
        optimize "On"

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        
    }

    includedirs 
    {
        "GameEngine/vendor/spdlog/include",
        "GameEngine/src",
        "GameEngine/vendor",
        "%{IncludeDir.glm}"
    }

    links
    {
        "GameEngine"
    }

    filter "system:windows"
        cppdialect "C++14"
        staticruntime "Off"
        systemversion "10.0.22000.0"

        defines{
            "GE_PLATFORM_WINDOWS",
         }

    filter "configurations:Debug"
        defines "GE_DEBUG"
        buildoptions "/MDd"
        symbols "On"

    filter "configurations:Release"
        defines "GE_RELEASE"
        buildoptions "/MD"
        optimize "On"

    filter "configurations:Dist"
        defines "GE_DIST"
        buildoptions "/MD"
        optimize "On"