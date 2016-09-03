set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

SET(src_lua
    ../external/lua/src/lapi.c
    ../external/lua/src/lauxlib.c
    ../external/lua/src/lbaselib.c
    ../external/lua/src/lbitlib.c
    ../external/lua/src/lcode.c
    ../external/lua/src/lcorolib.c
    ../external/lua/src/lctype.c
    ../external/lua/src/ldblib.c
    ../external/lua/src/ldebug.c
    ../external/lua/src/ldo.c
    ../external/lua/src/ldump.c
    ../external/lua/src/lfunc.c
    ../external/lua/src/lgc.c
    ../external/lua/src/linit.c
    ../external/lua/src/liolib.c
    ../external/lua/src/llex.c
    ../external/lua/src/lmathlib.c
    ../external/lua/src/lmem.c
    ../external/lua/src/loadlib.c
    ../external/lua/src/lobject.c
    ../external/lua/src/lopcodes.c
    ../external/lua/src/loslib.c
    ../external/lua/src/lparser.c
    ../external/lua/src/lstate.c
    ../external/lua/src/lstring.c
    ../external/lua/src/lstrlib.c
    ../external/lua/src/ltable.c
    ../external/lua/src/ltablib.c
    ../external/lua/src/ltm.c
    ../external/lua/src/lundump.c
    ../external/lua/src/lutf8lib.c
    ../external/lua/src/lvm.c
    ../external/lua/src/lzio.c
)

################################################################################

ADD_EXECUTABLE(lua_selene lua_selene.cc ${src_lua})
TARGET_INCLUDE_DIRECTORIES(lua_selene
                           PRIVATE "../external/selene/include"
                           PRIVATE "../external/lua/src"
                           )
SET_PROPERTY(TARGET lua_selene PROPERTY CXX_STANDARD 11)
SET_PROPERTY(TARGET lua_selene PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################

ADD_EXECUTABLE(lua_sol2 lua_sol2.cc ${src_lua})
TARGET_INCLUDE_DIRECTORIES(lua_sol2
                           PRIVATE "../external/sol2"
                           PRIVATE "../external/lua/src"
                           )
SET_PROPERTY(TARGET lua_sol2 PROPERTY CXX_STANDARD 14)
SET_PROPERTY(TARGET lua_sol2 PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################

ADD_EXECUTABLE(lua_custom lua_custom.cc ${src_lua})
TARGET_INCLUDE_DIRECTORIES(lua_custom
                           PRIVATE "../external/lua/src"
                           )
SET_PROPERTY(TARGET lua_custom PROPERTY CXX_STANDARD 11)
SET_PROPERTY(TARGET lua_custom PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################

FIND_PACKAGE(LuaJIT)
if( ${LUAJIT_FOUND} )
  MESSAGE(STATUS "Including luajit tests")

  # selene require 5.2 but luajit is only 5.1 :(
  ADD_EXECUTABLE(luajit_selene lua_selene.cc)
  # TARGET_INCLUDE_DIRECTORIES(luajit_selene
  #                            PRIVATE "../external/selene/include"
  #                            PRIVATE ${LUAJIT_INCLUDE_DIR}
  #                            )
  # TARGET_LINK_LIBRARIES(luajit_selene ${LUAJIT_LIBRARIES})
  # SET_PROPERTY(TARGET luajit_selene PROPERTY CXX_STANDARD 11)
  # SET_PROPERTY(TARGET luajit_selene PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)


  ADD_EXECUTABLE(luajit_sol2 lua_sol2.cc)
  TARGET_INCLUDE_DIRECTORIES(luajit_sol2
                             PRIVATE "../external/sol2"
                             PRIVATE ${LUAJIT_INCLUDE_DIR}
                             )
  TARGET_LINK_LIBRARIES(luajit_sol2 ${LUAJIT_LIBRARIES})
  SET_PROPERTY(TARGET luajit_sol2 PROPERTY CXX_STANDARD 14)
  SET_PROPERTY(TARGET luajit_sol2 PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

################################################################################

FIND_PACKAGE(V8)
IF(${V8_FOUND})
  MESSAGE(STATUS "Including v8 test")
  ADD_EXECUTABLE(js_v8 js_v8.cc)
  TARGET_INCLUDE_DIRECTORIES(js_v8
                             PRIVATE ${V8_INCLUDE_DIR}
                             )
  TARGET_LINK_LIBRARIES(js_v8 ${V8_LIBRARY})
  SET_PROPERTY(TARGET js_v8 PROPERTY CXX_STANDARD 11)
  SET_PROPERTY(TARGET js_v8 PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)
ENDIF(${V8_FOUND})

################################################################################