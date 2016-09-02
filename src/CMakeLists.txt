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

SET(src_luabind
    ../external/luabind-0.9.1/src/class.cpp
    ../external/luabind-0.9.1/src/class_info.cpp
    ../external/luabind-0.9.1/src/class_registry.cpp
    ../external/luabind-0.9.1/src/class_rep.cpp
    ../external/luabind-0.9.1/src/create_class.cpp
    ../external/luabind-0.9.1/src/error.cpp
    ../external/luabind-0.9.1/src/exception_handler.cpp
    ../external/luabind-0.9.1/src/function.cpp
    ../external/luabind-0.9.1/src/inheritance.cpp
    ../external/luabind-0.9.1/src/link_compatibility.cpp
    ../external/luabind-0.9.1/src/object_rep.cpp
    ../external/luabind-0.9.1/src/open.cpp
    ../external/luabind-0.9.1/src/pcall.cpp
    ../external/luabind-0.9.1/src/scope.cpp
    ../external/luabind-0.9.1/src/stack_content_by_name.cpp
    ../external/luabind-0.9.1/src/weak_ref.cpp
    ../external/luabind-0.9.1/src/wrapper_base.cpp
    )

SET(src_slb
    ../external/slb/src/slb3.cc
    ../external/slb/src/slb3_extra.cc
    )

SET(src_oolua
    ../external/oolua-2.0.1/src/class_from_stack.cpp
    ../external/oolua-2.0.1/src/oolua.cpp
    ../external/oolua-2.0.1/src/oolua_check_result.cpp
    ../external/oolua-2.0.1/src/oolua_chunk.cpp
    ../external/oolua-2.0.1/src/oolua_error.cpp
    ../external/oolua-2.0.1/src/oolua_exception.cpp
    ../external/oolua-2.0.1/src/oolua_function.cpp
    ../external/oolua-2.0.1/src/oolua_helpers.cpp
    ../external/oolua-2.0.1/src/oolua_open.cpp
    ../external/oolua-2.0.1/src/oolua_pull.cpp
    ../external/oolua-2.0.1/src/oolua_push.cpp
    ../external/oolua-2.0.1/src/oolua_ref.cpp
    ../external/oolua-2.0.1/src/oolua_registration.cpp
    ../external/oolua-2.0.1/src/oolua_script.cpp
    ../external/oolua-2.0.1/src/oolua_stack_dump.cpp
    ../external/oolua-2.0.1/src/oolua_string.cpp
    ../external/oolua-2.0.1/src/oolua_table.cpp
    ../external/oolua-2.0.1/src/proxy_storage.cpp
    ../external/oolua-2.0.1/src/push_pointer_internal.cpp
    ../external/oolua-2.0.1/src/stack_get.cpp
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

ADD_EXECUTABLE(lua_luabind lua_luabind.cc ${src_lua} ${src_luabind})
TARGET_INCLUDE_DIRECTORIES(lua_luabind
                           PRIVATE "../external/luabind-0.9.1/"
                           PRIVATE "../external/lua/src"
                           )
SET_PROPERTY(TARGET lua_luabind PROPERTY CXX_STANDARD 11)
SET_PROPERTY(TARGET lua_luabind PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################

ADD_EXECUTABLE(lua_slb lua_slb.cc ${src_lua} ${src_slb})
TARGET_INCLUDE_DIRECTORIES(lua_slb
                           PRIVATE "../external/slb/include"
                           PRIVATE "../external/lua/src"
                           )
SET_PROPERTY(TARGET lua_slb PROPERTY CXX_STANDARD 11)
SET_PROPERTY(TARGET lua_slb PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################

ADD_EXECUTABLE(lua_oolua lua_oolua.cc ${src_lua} ${src_oolua})
TARGET_INCLUDE_DIRECTORIES(lua_oolua
                           PRIVATE "../external/oolua-2.0.1/include"
                           PRIVATE "../external/lua/src"
                           )
SET_PROPERTY(TARGET lua_oolua PROPERTY CXX_STANDARD 11)
SET_PROPERTY(TARGET lua_oolua PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)

################################################################################

FIND_PACKAGE(LuaJIT)
if( ${LUAJIT_FOUND} )
  MESSAGE(STATUS "Including luajit tests")

  # selene require 5.2 but luajit is only 5.1 :(
  # ADD_EXECUTABLE(luajit_selene lua_selene.cc)
  # TARGET_INCLUDE_DIRECTORIES(luajit_selene
  #                            PRIVATE "../external/selene/include"
  #                            PRIVATE ${LUAJIT_INCLUDE_DIR}
  #                            )
  # TARGET_LINK_LIBRARIES(luajit_selene ${LUAJIT_LIBRARIES})
  # SET_PROPERTY(TARGET luajit_selene PROPERTY CXX_STANDARD 11)
  # SET_PROPERTY(TARGET luajit_selene PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)
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