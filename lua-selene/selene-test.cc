#include <iostream>

#include "selene.h"

void TestFunc() {
  std::cout << "I'm a function in C++\n";
}

struct Bar {
  int x;
  Bar(int x_) : x(x_) {}
  int AddThis(int y) const { return x + y; }
};

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

  std::cout << "\nClass test\n";
  state["Bar"].SetClass<Bar, int>
      ("add_this", &Bar::AddThis);
  int use_bar_class = state["use_bar_class"]();
  int take_bar_class = state["take_bar_class"](Bar(5));
  Bar return_bar_class_bar = state["return_bar_class"]();
  int return_bar_class = return_bar_class_bar.AddThis(2);
  std::cout << "use_bar_class(7): " << use_bar_class << "\n";
  std::cout << "take_bar_class(7): " << take_bar_class << "\n";
  std::cout << "return_bar_class(7): " << return_bar_class << "\n";

  return 0;
}
