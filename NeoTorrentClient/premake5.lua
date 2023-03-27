project "NeoTorrentClient"
   kind "ConsoleApp"
   language "C++"
   cppdialect "C++20"
   targetdir "bin/%{cfg.buildcfg}"
   staticruntime "off"

   pchheader "NTCpch.h"
   pchsource "src/NTCpch.cpp"

   files { "src/**.h", "src/**.cpp" }


   defines
   {
       "_CRT_SECURE_NO_WARNINGS"
   }

   includedirs
   {
      "../vendor/imgui",
      "../vendor/glfw/include",

      "../Walnut/src",

      "%{IncludeDir.VulkanSDK}",
      "%{IncludeDir.glm}",

      "src",
      "vendor/spdlog/include",
   }

    links
    {
        "Walnut"
    }

   targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
   objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

   filter "system:windows"
      systemversion "latest"
      defines { "NTC_PLATFORM_WINDOWS" }

   filter "configurations:Debug"
      defines 
      {
         "NTC_DEBUG",
         "NTC_PROFILE"
      }
      runtime "Debug"
      symbols "On"

   filter "configurations:Release"
   defines 
   {
       "NTC_RELEASE",
       "NTC_PROFILE"
   }
      runtime "Release"
      optimize "On"
      symbols "On"

   filter "configurations:Dist"
      kind "WindowedApp"
      defines { "NTC_DIST" }
      runtime "Release"
      optimize "On"
      symbols "Off"