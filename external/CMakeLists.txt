FIND_PACKAGE(Threads REQUIRED)
FIND_PACKAGE(LuaJIT)

###################################################################################################
# lua

add_library(lua STATIC
    lua/src/lapi.c
    lua/src/lauxlib.c
    lua/src/lbaselib.c
    lua/src/lbitlib.c
    lua/src/lcode.c
    lua/src/lcorolib.c
    lua/src/lctype.c
    lua/src/ldblib.c
    lua/src/ldebug.c
    lua/src/ldo.c
    lua/src/ldump.c
    lua/src/lfunc.c
    lua/src/lgc.c
    lua/src/linit.c
    lua/src/liolib.c
    lua/src/llex.c
    lua/src/lmathlib.c
    lua/src/lmem.c
    lua/src/loadlib.c
    lua/src/lobject.c
    lua/src/lopcodes.c
    lua/src/loslib.c
    lua/src/lparser.c
    lua/src/lstate.c
    lua/src/lstring.c
    lua/src/lstrlib.c
    lua/src/ltable.c
    lua/src/ltablib.c
    lua/src/ltm.c
    lua/src/lundump.c
    lua/src/lutf8lib.c
    lua/src/lvm.c
    lua/src/lzio.c
)
target_include_directories(lua
    PUBLIC
        lua/src
)


###################################################################################################
# angelscript
add_library(angelscript STATIC
    angelscript_2.35.0/angelscript/source/as_array.h
    angelscript_2.35.0/angelscript/source/as_atomic.cpp
    angelscript_2.35.0/angelscript/source/as_atomic.h
    angelscript_2.35.0/angelscript/source/as_builder.cpp
    angelscript_2.35.0/angelscript/source/as_builder.h
    angelscript_2.35.0/angelscript/source/as_bytecode.cpp
    angelscript_2.35.0/angelscript/source/as_bytecode.h
    angelscript_2.35.0/angelscript/source/as_callfunc_arm.cpp
    angelscript_2.35.0/angelscript/source/as_callfunc_arm_gcc.S
    angelscript_2.35.0/angelscript/source/as_callfunc_arm_msvc.asm
    angelscript_2.35.0/angelscript/source/as_callfunc_arm_vita.S
    angelscript_2.35.0/angelscript/source/as_callfunc_arm_xcode.S
    angelscript_2.35.0/angelscript/source/as_callfunc.cpp
    angelscript_2.35.0/angelscript/source/as_callfunc.h
    angelscript_2.35.0/angelscript/source/as_callfunc_mips.cpp
    angelscript_2.35.0/angelscript/source/as_callfunc_ppc_64.cpp
    angelscript_2.35.0/angelscript/source/as_callfunc_ppc.cpp
    angelscript_2.35.0/angelscript/source/as_callfunc_sh4.cpp
    angelscript_2.35.0/angelscript/source/as_callfunc_x64_gcc.cpp
    angelscript_2.35.0/angelscript/source/as_callfunc_x64_mingw.cpp
    angelscript_2.35.0/angelscript/source/as_callfunc_x64_msvc_asm.asm
    angelscript_2.35.0/angelscript/source/as_callfunc_x64_msvc.cpp
    angelscript_2.35.0/angelscript/source/as_callfunc_x86.cpp
    angelscript_2.35.0/angelscript/source/as_callfunc_xenon.cpp
    angelscript_2.35.0/angelscript/source/as_compiler.cpp
    angelscript_2.35.0/angelscript/source/as_compiler.h
    angelscript_2.35.0/angelscript/source/as_configgroup.cpp
    angelscript_2.35.0/angelscript/source/as_configgroup.h
    angelscript_2.35.0/angelscript/source/as_config.h
    angelscript_2.35.0/angelscript/source/as_context.cpp
    angelscript_2.35.0/angelscript/source/as_context.h
    angelscript_2.35.0/angelscript/source/as_criticalsection.h
    angelscript_2.35.0/angelscript/source/as_datatype.cpp
    angelscript_2.35.0/angelscript/source/as_datatype.h
    angelscript_2.35.0/angelscript/source/as_debug.h
    angelscript_2.35.0/angelscript/source/as_gc.cpp
    angelscript_2.35.0/angelscript/source/as_gc.h
    angelscript_2.35.0/angelscript/source/as_generic.cpp
    angelscript_2.35.0/angelscript/source/as_generic.h
    angelscript_2.35.0/angelscript/source/as_globalproperty.cpp
    angelscript_2.35.0/angelscript/source/as_map.h
    angelscript_2.35.0/angelscript/source/as_memory.cpp
    angelscript_2.35.0/angelscript/source/as_memory.h
    angelscript_2.35.0/angelscript/source/as_module.cpp
    angelscript_2.35.0/angelscript/source/as_module.h
    angelscript_2.35.0/angelscript/source/as_namespace.h
    angelscript_2.35.0/angelscript/source/as_objecttype.cpp
    angelscript_2.35.0/angelscript/source/as_objecttype.h
    angelscript_2.35.0/angelscript/source/as_outputbuffer.cpp
    angelscript_2.35.0/angelscript/source/as_outputbuffer.h
    angelscript_2.35.0/angelscript/source/as_parser.cpp
    angelscript_2.35.0/angelscript/source/as_parser.h
    angelscript_2.35.0/angelscript/source/as_property.h
    angelscript_2.35.0/angelscript/source/as_restore.cpp
    angelscript_2.35.0/angelscript/source/as_restore.h
    angelscript_2.35.0/angelscript/source/as_scriptcode.cpp
    angelscript_2.35.0/angelscript/source/as_scriptcode.h
    angelscript_2.35.0/angelscript/source/as_scriptengine.cpp
    angelscript_2.35.0/angelscript/source/as_scriptengine.h
    angelscript_2.35.0/angelscript/source/as_scriptfunction.cpp
    angelscript_2.35.0/angelscript/source/as_scriptfunction.h
    angelscript_2.35.0/angelscript/source/as_scriptnode.cpp
    angelscript_2.35.0/angelscript/source/as_scriptnode.h
    angelscript_2.35.0/angelscript/source/as_scriptobject.cpp
    angelscript_2.35.0/angelscript/source/as_scriptobject.h
    angelscript_2.35.0/angelscript/source/as_string.cpp
    angelscript_2.35.0/angelscript/source/as_string.h
    angelscript_2.35.0/angelscript/source/as_string_util.cpp
    angelscript_2.35.0/angelscript/source/as_string_util.h
    angelscript_2.35.0/angelscript/source/as_symboltable.h
    angelscript_2.35.0/angelscript/source/as_texts.h
    angelscript_2.35.0/angelscript/source/as_thread.cpp
    angelscript_2.35.0/angelscript/source/as_thread.h
    angelscript_2.35.0/angelscript/source/as_tokendef.h
    angelscript_2.35.0/angelscript/source/as_tokenizer.cpp
    angelscript_2.35.0/angelscript/source/as_tokenizer.h
    angelscript_2.35.0/angelscript/source/as_typeinfo.cpp
    angelscript_2.35.0/angelscript/source/as_typeinfo.h
    angelscript_2.35.0/angelscript/source/as_variablescope.cpp
    angelscript_2.35.0/angelscript/source/as_variablescope.h
    angelscript_2.35.0/add_on/contextmgr/contextmgr.cpp
    angelscript_2.35.0/add_on/datetime/datetime.cpp
    angelscript_2.35.0/add_on/debugger/debugger.cpp
    angelscript_2.35.0/add_on/scriptany/scriptany.cpp
    angelscript_2.35.0/add_on/scriptarray/scriptarray.cpp
    angelscript_2.35.0/add_on/scriptbuilder/scriptbuilder.cpp
    angelscript_2.35.0/add_on/scriptdictionary/scriptdictionary.cpp
    angelscript_2.35.0/add_on/scriptfile/scriptfile.cpp
    angelscript_2.35.0/add_on/scriptfile/scriptfilesystem.cpp
    angelscript_2.35.0/add_on/scriptgrid/scriptgrid.cpp
    angelscript_2.35.0/add_on/scripthandle/scripthandle.cpp
    angelscript_2.35.0/add_on/scripthelper/scripthelper.cpp
    angelscript_2.35.0/add_on/scriptmath/scriptmathcomplex.cpp
    angelscript_2.35.0/add_on/scriptmath/scriptmath.cpp
    angelscript_2.35.0/add_on/scriptstdstring/scriptstdstring.cpp
    angelscript_2.35.0/add_on/scriptstdstring/scriptstdstring_utils.cpp
    angelscript_2.35.0/add_on/serializer/serializer.cpp
    angelscript_2.35.0/add_on/weakref/weakref.cpp
)
target_include_directories(angelscript
    PUBLIC
        angelscript_2.35.0/angelscript/include
        angelscript_2.35.0/add_on
)
target_compile_definitions(angelscript
    PUBLIC
        AS_USE_NAMESPACE
)
target_link_libraries(angelscript
    PUBLIC
        ${CMAKE_THREAD_LIBS_INIT}
)
target_compile_features(angelscript PRIVATE cxx_std_17)
# -fno-strict-aliasing



###################################################################################################
add_library(sol2 INTERFACE)
target_include_directories(sol2
    INTERFACE
        sol2
)



###################################################################################################
add_library(chai INTERFACE)
target_include_directories(chai
    INTERFACE
        ChaiScript-6.1.0/include
)
target_link_libraries(chai
    INTERFACE
        ${CMAKE_DL_LIBS}
        ${CMAKE_THREAD_LIBS_INIT}
)


###################################################################################################

if( ${LUAJIT_FOUND} )
    add_library(luajit INTERFACE)
    target_include_directories(luajit
        INTERFACE
            ${LUAJIT_INCLUDE_DIR}
    )
    target_link_libraries(luajit
        INTERFACE
            ${LUAJIT_LIBRARIES}
    )
endif()

