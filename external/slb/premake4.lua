-- Copyright 2013 Jose L. Hidalgo <pplux@pplux.com
-- all rights reserved
--
-- script to generate/test SLB 3.0

solution "SLB"
  configurations {"Debug", "Release", "Debug-no-rtti", "Release-no-rtti"}
  location("build")
  targetdir("build/bin")
  
  language "C++"

  includedirs { "include/" }

  configuration "Debug*"
    targetsuffix "_debug"
    defines {"_DEBUG"}
    flags { "Symbols" }

  configuration "Debug-no-rtti"
    targetsuffix "_debug_no_rtti"

  configuration "Release-no-rtti"
    targetsuffix "_no_rtti"

  configuration "macosx or linux"
    buildoptions {"-Wall", "-Wextra", "-Wfatal-errors" }
    buildoptions {"-g", "-O0" }
    
  configuration "Release"
    flags { "Optimize" }

  configuration {"vs2010"}
    defines {"_CRT_SECURE_NO_WARNINGS=1" }
    buildoptions {"/W3" }

  configuration { "Debug", "vs2010" }
    linkoptions {"/NODEFAULTLIB:msvcrt.lib"}

  configuration {"macos or linux", "*-no-rtti"}
    buildoptions { "-fno-rtti" }

  configuration {"vs2010", "*-no-rtti"}
    buildoptions { "/GR-" }

  configuration {"vs2010", "Debug or Release"}
    buildoptions { "/GR" }

project "Test"
  kind "ConsoleApp"
  includedirs {"deps/gtest/include", "deps/gtest"}
  includedirs {"deps/lua/src/"}
  files {"src/*.cc", "test/*.cc"}
  files {"deps/gtest/src/gtest-all.cc"}
  files {"deps/lua/src/*.c"}
  files {"include/**.h"}
  excludes { "deps/lua/src/lua.c","deps/lua/src/luac.c" }

--[[ One Day I will support lua-JIT... but since it doesnt support most part of
     lua 5.2.2.... I can't right now.
project "Test-JIT"
  kind "ConsoleApp"
  defines {"LUAJIT_ENABLE_LUA52COMPAT"}
  includedirs {"deps/gtest/include", "deps/gtest"}
  includedirs {"deps/lua-jit/src"}
  files {"src/*.cc", "test/*.cc"}
  files {"deps/gtest/src/gtest-all.cc"}
  files {"deps/lua-jit/src/ljamalg.c"}
  files {"include/**.h"}
  excludes { "deps/lua/src/lua.c","deps/lua/src/luac.c" }
]]
  
