-- premake5.lua
solution "eMMOji"
  configurations { "Debug", "Release" }
  location ("bld/" .. _ACTION)

project "eMMOji"
  kind "ConsoleApp"
  language "C++"
  targetdir "./"
  debugdir "./"
  location ("bld/" .. _ACTION)

  files { "src/**.cpp", "src/**.hpp" }
  includedirs { "src/", "src/*", "ext/", "ext/asio-1.10.6/include" }

  defines { "ASIO_STANDALONE" }

  configuration { "linux" }
    buildoptions { "-std=c++11" }
    links { "pthread", "z", "ssl", "crypto", "rt" }
  configuration "windows"
    links { "ws2_32", "Shlwapi", "notelemetry.obj" }

  filter "configurations:Debug"
    defines { "DEBUG" }
    flags { "Symbols" }

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
