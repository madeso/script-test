#include <iostream>

#include "selene.h"

int main() {
  using namespace sel;
  State state{true};
  if( false == state.Load("test.lua") ) {
    std::cerr << "Failed to load test.lua\n";
    return 1;
  }

  // accessing variables
  const int foo = state["foo"];
  const std::string bar = state["bar"][3];
  const std::string key = state["bar"]["key"];
  std::cout << "foo(4) " << foo << "\n";
  std::cout << "bar(hi) " << bar << "\n";
  std::cout << "key(there) " << key << "\n";

  return 0;
}
