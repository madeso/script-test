#include <iostream>

#include "selene.h"

void TestFunc() {
  std::cout << "I'm a function in C++\n";
}

int main() {
  using namespace sel;
  State state{true};
  if( false == state.Load("test.lua") ) {
    std::cerr << "Failed to load test.lua\n";
    return 1;
  }

  std::cout << "\nAccessing variables\n";
  const int foo = state["foo"];
  const std::string bar = state["bar"][3];
  const std::string key = state["bar"]["key"];
  std::cout << "foo(4): " << foo << "\n";
  std::cout << "bar(hi): " << bar << "\n";
  std::cout << "key(there): " << key << "\n";

  std::cout << "\nCalling lua functions\n";

  state["foo"]();

  int result = state["add"](5, 2);
  std::cout << "result(7) " << result << "\n";
  int sum, difference;
  sel::tie(sum, difference) = state["sum_and_difference"](3, 1);
  std::cout << "sum(4): " << sum << "\ndiff(2): " << difference << "\n";

  result = state["mytable"]["foo"]();
  std::cout << "result(4): " << result << "\n";

  std::cout << "\nCalling C++ function from lua\n";
  state["test_func"] = &TestFunc;
  state("test_func()");

  return 0;
}
