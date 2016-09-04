set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")
FIND_PACKAGE(Threads)

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

SET(src_angelscript
    ../external/angelscript_2.31.1/angelscript/source/as_array.h
    ../external/angelscript_2.31.1/angelscript/source/as_atomic.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_atomic.h
    ../external/angelscript_2.31.1/angelscript/source/as_builder.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_builder.h
    ../external/angelscript_2.31.1/angelscript/source/as_bytecode.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_bytecode.h
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_arm.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_arm_gcc.S
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_arm_msvc.asm
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_arm_vita.S
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_arm_xcode.S
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc.h
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_mips.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_ppc_64.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_ppc.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_sh4.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_x64_gcc.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_x64_mingw.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_x64_msvc_asm.asm
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_x64_msvc.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_x86.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_callfunc_xenon.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_compiler.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_compiler.h
    ../external/angelscript_2.31.1/angelscript/source/as_configgroup.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_configgroup.h
    ../external/angelscript_2.31.1/angelscript/source/as_config.h
    ../external/angelscript_2.31.1/angelscript/source/as_context.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_context.h
    ../external/angelscript_2.31.1/angelscript/source/as_criticalsection.h
    ../external/angelscript_2.31.1/angelscript/source/as_datatype.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_datatype.h
    ../external/angelscript_2.31.1/angelscript/source/as_debug.h
    ../external/angelscript_2.31.1/angelscript/source/as_gc.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_gc.h
    ../external/angelscript_2.31.1/angelscript/source/as_generic.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_generic.h
    ../external/angelscript_2.31.1/angelscript/source/as_globalproperty.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_map.h
    ../external/angelscript_2.31.1/angelscript/source/as_memory.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_memory.h
    ../external/angelscript_2.31.1/angelscript/source/as_module.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_module.h
    ../external/angelscript_2.31.1/angelscript/source/as_namespace.h
    ../external/angelscript_2.31.1/angelscript/source/as_objecttype.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_objecttype.h
    ../external/angelscript_2.31.1/angelscript/source/as_outputbuffer.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_outputbuffer.h
    ../external/angelscript_2.31.1/angelscript/source/as_parser.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_parser.h
    ../external/angelscript_2.31.1/angelscript/source/as_property.h
    ../external/angelscript_2.31.1/angelscript/source/as_restore.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_restore.h
    ../external/angelscript_2.31.1/angelscript/source/as_scriptcode.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_scriptcode.h
    ../external/angelscript_2.31.1/angelscript/source/as_scriptengine.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_scriptengine.h
    ../external/angelscript_2.31.1/angelscript/source/as_scriptfunction.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_scriptfunction.h
    ../external/angelscript_2.31.1/angelscript/source/as_scriptnode.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_scriptnode.h
    ../external/angelscript_2.31.1/angelscript/source/as_scriptobject.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_scriptobject.h
    ../external/angelscript_2.31.1/angelscript/source/as_string.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_string.h
    ../external/angelscript_2.31.1/angelscript/source/as_string_util.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_string_util.h
    ../external/angelscript_2.31.1/angelscript/source/as_symboltable.h
    ../external/angelscript_2.31.1/angelscript/source/as_texts.h
    ../external/angelscript_2.31.1/angelscript/source/as_thread.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_thread.h
    ../external/angelscript_2.31.1/angelscript/source/as_tokendef.h
    ../external/angelscript_2.31.1/angelscript/source/as_tokenizer.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_tokenizer.h
    ../external/angelscript_2.31.1/angelscript/source/as_typeinfo.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_typeinfo.h
    ../external/angelscript_2.31.1/angelscript/source/as_variablescope.cpp
    ../external/angelscript_2.31.1/angelscript/source/as_variablescope.h
    ../external/angelscript_2.31.1/add_on/contextmgr/contextmgr.cpp
    ../external/angelscript_2.31.1/add_on/datetime/datetime.cpp
    ../external/angelscript_2.31.1/add_on/debugger/debugger.cpp
    ../external/angelscript_2.31.1/add_on/scriptany/scriptany.cpp
    ../external/angelscript_2.31.1/add_on/scriptarray/scriptarray.cpp
    ../external/angelscript_2.31.1/add_on/scriptbuilder/scriptbuilder.cpp
    ../external/angelscript_2.31.1/add_on/scriptdictionary/scriptdictionary.cpp
    ../external/angelscript_2.31.1/add_on/scriptfile/scriptfile.cpp
    ../external/angelscript_2.31.1/add_on/scriptfile/scriptfilesystem.cpp
    ../external/angelscript_2.31.1/add_on/scriptgrid/scriptgrid.cpp
    ../external/angelscript_2.31.1/add_on/scripthandle/scripthandle.cpp
    ../external/angelscript_2.31.1/add_on/scripthelper/scripthelper.cpp
    ../external/angelscript_2.31.1/add_on/scriptmath/scriptmathcomplex.cpp
    ../external/angelscript_2.31.1/add_on/scriptmath/scriptmath.cpp
    ../external/angelscript_2.31.1/add_on/scriptstdstring/scriptstdstring.cpp
    ../external/angelscript_2.31.1/add_on/scriptstdstring/scriptstdstring_utils.cpp
    ../external/angelscript_2.31.1/add_on/serializer/serializer.cpp
    ../external/angelscript_2.31.1/add_on/weakref/weakref.cpp
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

ADD_EXECUTABLE(angelscript angelscript.cc ${src_angelscript})
TARGET_INCLUDE_DIRECTORIES(angelscript
                           PRIVATE "../external/angelscript_2.31.1/angelscript/include"
                           PRIVATE "../external/angelscript_2.31.1/add_on"
                           )
SET_PROPERTY(TARGET angelscript PROPERTY CXX_STANDARD 11)
SET_PROPERTY(TARGET angelscript PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)
TARGET_COMPILE_DEFINITIONS(angelscript PRIVATE AS_USE_NAMESPACE)
TARGET_LINK_LIBRARIES(angelscript ${CMAKE_THREAD_LIBS_INIT})
# -fno-strict-aliasing

################################################################################

ADD_EXECUTABLE(chai chai.cc)
TARGET_INCLUDE_DIRECTORIES(chai
                           PRIVATE "../external/ChaiScript-5.8.1/include"
                           )
SET_PROPERTY(TARGET chai PROPERTY CXX_STANDARD 11)
SET_PROPERTY(TARGET chai PROPERTY CMAKE_CXX_STANDARD_REQUIRED ON)
TARGET_LINK_LIBRARIES(chai ${CMAKE_DL_LIBS})
TARGET_LINK_LIBRARIES(chai ${CMAKE_THREAD_LIBS_INIT})

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