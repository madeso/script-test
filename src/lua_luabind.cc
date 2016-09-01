#include <iostream>
#include "shared.h"

extern "C"
{
#include "lua.h"
}

#include <luabind/luabind.hpp>

void greet()
{
  std::cout << "hello world!\n";
}

extern "C" int init(lua_State* L)
{
  using namespace luabind;

  open(L);

  module(L)
  [
      def("greet", &greet)
  ];

  return 0;
}


int main() {
  std::cout << "Hello world!\n";
  return 0;
}
