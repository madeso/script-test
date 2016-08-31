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

ADD_EXECUTABLE(lua_selene lua_selene.cc ${src_lua})
TARGET_INCLUDE_DIRECTORIES(lua_selene
                           PRIVATE "../external/selene/include"
                           PRIVATE "../external/lua/src"
                           )
SET_PROPERTY(TARGET lua_selene PROPERTY CXX_STANDARD 11)
SET_PROPERTY(TARGET lua_selene PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
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