#include <iostream>
#include "shared.h"

extern "C"
{
#include "lua.h"
}

#include <luabind/luabind.hpp>

int main() {
  std::cout << "Hello world!\n";
  return 0;
}
