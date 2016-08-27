#include <iostream>

#include "selene.h"

int main() {
  using namespace sel;
  State state{true};
  if( false == state.Load("test.lua") ) {
    std::cerr << "Failed to load test\n";
    return 1;
  }
  return 0;
}
