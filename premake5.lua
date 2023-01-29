workspace "NeoTorrentClient"
    architecture "x86_64"
    startproject "NTCBackend"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "NTCBackend"
    location "NTCBackend"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "NTCpch.h"
    pchsource "%{prj.name}/src/NTCpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
    }

    filter "configurations:Debug"
    defines 
    {
        "NTC_DEBUG",
        "NTC_PROFILE"
    }
    runtime "Debug"
    symbols "on"

filter "configurations:Release"
    defines 
    {
        "NTC_RELEASE",
        "NTC_PROFILE"
    }
    runtime "Release"
    optimize "on"

filter "configurations:Dist"
    defines "NTC_DIST"
    runtime "Release"
    optimize "on"