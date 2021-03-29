cmake_minimum_required (VERSION 3.15)
set_property(GLOBAL PROPERTY USE_FOLDERS OFF)
project (scripts C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

include(cmake/StandardProjectSettings.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/Sanitizers.cmake)

add_library(project_options INTERFACE)
add_library(project_warnings INTERFACE)

target_compile_features(project_options INTERFACE cxx_std_17)
# set_project_warnings(project_warnings)
enable_sanitizers(project_options)

add_subdirectory(external)


add_library(shared STATIC
    src/shared.cc src/shared.h
)
target_link_libraries(shared
    PUBLIC
        project_options
        project_warnings
)
target_compile_definitions(shared
    PUBLIC
        SCRIPTS="${CMAKE_SOURCE_DIR}/scripts/"
)

# add_subdirectory(src)

add_executable(test_lua src/lua_sol2.cc)
target_link_libraries(test_lua
    PUBLIC
        shared
        sol2
        lua
)

add_executable(test_angelscript src/angelscript.cc)
target_link_libraries(test_angelscript
    PUBLIC
        shared
        angelscript
)

add_executable(test_chai src/chai.cc)
target_link_libraries(test_chai
    PUBLIC
        shared
        chai
)
target_compile_definitions(test_chai
    PUBLIC
        CHAISCRIPT_NO_THREADS
)

add_executable(test_chai_threadsafe src/chai.cc)
target_link_libraries(test_chai_threadsafe
    PUBLIC
        shared
        chai
)


