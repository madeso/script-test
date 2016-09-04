## lua + Selene - finished
* Nothing to build and easy to bind
* Constructors can only take one argument
* OK, but doesn't support LuaJIT.

## Javascript + v8 - WIP
* custom build tool you just can't git the source and build, there are several custom steps
* various API changes

 
## lua + luabind - aborted
* download from sourceforge
* low activity(last update june 2013)
* seems to be focused on extending lua as a dll, not embedding
* uses boost.build but seems to be files can just be included
* requires boost when compiling

## lua - simple lua binder 3 - aborted
* seems pretty dead, only google code hosting as far as I can tell
* needed to add a space ion a few places to fix literal operators/string concat errors
* neither examples nor documentation and the few tests point to a API that seems to be a "safe" wrapper over the standard API.

## lua + oolua - aborted
* uses non standard include lua/lua.h
* mixes custom library with lua code, doesn't deal with the "C" issues and potential for screwups
* seemingly unable to get the return value
* seems to be focused on extending instead of embedding

## lua + tolua++ - aborted
* seems dead, last update seems to be in 2012
* C library, not of any use

## lua + MLuaBind - aborted
* sourceforge download
* dead? last update 2013
* requires loki, also hosted by sourceforge, last updated in 2009, also refers to Loki instead of loki
* various compiler issues and other errors relating to older c++ and never building on linux

## lua + luabridge - aborted
* last update 2013
* needed to fix source for compiler error
* one main feature is that there is only one include, but you have to include lua headers too, or you will get compiler errors and this isn't stated anywhere,
* only for binding, doesn't support variable lookup, for example
* you have to use lua calls to use the library
* cumbersome lua like c++ binding, doesn't look nice at all

## lua + sol2
* seems to need some parts of c++14 (it says it is c++11/14 but only compiling with 11 results in errors)
* advanced template uses sometimes comfuses error highlightning in CLion

## chai
* slow compilation
* tightly connected to C++

