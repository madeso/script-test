#include <iostream>
#include "shared.h"

#include "oolua.h"

int main() {
  using namespace OOLUA;

  Script vm;
  if( false == vm.run_file("test.lua") ) {
    std::cerr << "Failed to load test.lua\n";
    return 1;
  }

  int foo = 42;
  if( false == get_global(vm, "foo", foo) ) {
    std::cerr << "Failed to get global foo: " << get_last_error(vm) << "\n";
  }
  std::cout << "foo(4): " << foo << "\n";

  if( false == vm.call("add", 3, 4) ) {
    std::cerr << "Failed to call add\n";
  }
  // how do I get the return values?

  return 0;
}
