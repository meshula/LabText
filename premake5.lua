
workspace "LabText"
    configurations { "Debug", "Release" }
    architecture "x86_64"
--    platforms { "Windows", "Linux", "macOS" }

--    targetdir ("local/bin/%{prj.name}/%{cfg.longname}")
--    objdir ("build/obj/%{prj.name}/%{cfg.longname}")

project "libLabText"
    kind "StaticLib"
    language "C++"

    includedirs { "src" }
    files { "src/**.h", "src/**.cpp", "src/**.c" }
    excludes { }

    configurations { "linux", "gmake" }
        buildoptions { "-std=c++11" }


    filter { "platforms:Windows" }
        system "windows"
        defines { "PLATFORM_WINDOWS" }

--    filter { "platforms:macOS" }
--        system "macosx"
--        buildoptions { "-std=c++11", "-stdlib=libc++" }
--        defines { "PLATFORM_DARWIN" }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "NDEBUG" }
        optimize "On"
