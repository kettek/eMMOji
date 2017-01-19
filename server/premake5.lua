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
  includedirs { "src/", "src/*", "ext/websocketpp", "ext/asio/asio/include" }

  defines { "ASIO_STANDALONE", "_WEBSOCKETPP_CPP11_FUNCTIONAL_", "_WEBSOCKETPP_CPP11_SYSTEM_ERROR_", "_WEBSOCKETPP_CPP11_RANDOM_DEVICE_", "_WEBSOCKETPP_CPP11_MEMORY_", "_WEBSOCKETPP_CPP11_STL_" }

  configuration { "linux" }
    buildoptions { "-std=c++11" }
    links { "pthread", "z", "ssl", "crypto", "rt" }
  configuration "windows"
    links { "notelemetry.obj" }

  filter "configurations:Debug"
    defines { "DEBUG" }
    flags { "Symbols" }

  filter "configurations:Release"
    defines { "NDEBUG" }
    optimize "On"
