#include <iostream>
#include <functional>
#include "shared.h"

#include <chaiscript/chaiscript.hpp>
#include <chaiscript/chaiscript_stdlib.hpp>

int main() {
  chaiscript::ChaiScript chai(chaiscript::Std_Lib::library());
  chai.eval_file("test.chai");

  auto foo = chai.eval<int>("foo");
  auto bar = chai.eval<std::string>("bar");

  std::cout << "foo(4): " << foo << "\n";
  std::cout << "bar(hi): " << bar << "\n";

  std::cout << "\nCalling script functions\n";

  auto ff = chai.eval<std::function<void (void)>>("ffoo");
  ff();

  auto fadd = chai.eval<std::function<int (int, int)>>("add");
  auto a = fadd(3, 2);
  int result = fadd(5, 2);
  std::cout << "result(7) " << result << "\n";

  std::cout << "\nCalling C++ function from script\n";
  chai.add(chaiscript::fun(&TestFunc), "test_func");
  chai.eval("test_func()");

  std::cout << "\nClass test\n";
  chai.add(chaiscript::user_type<Bar>(), "Bar");
  chai.add(chaiscript::constructor<Bar (int)>(), "Bar");
  chai.add(chaiscript::fun(&Bar::AddThis), "add_this");

  auto use_bar = chai.eval<std::function<int (void)>>("use_bar_class");
  auto take_bar = chai.eval<std::function<int (const Bar&)>>("take_bar_class");
  auto return_bar = chai.eval<std::function<Bar (void)>>("return_bar_class");

  int use_bar_class = use_bar();
  int take_bar_class = take_bar(Bar(5));
  Bar return_bar_class_bar = return_bar();
  int return_bar_class = return_bar_class_bar.AddThis(2);
  std::cout << "use_bar_class(7): " << use_bar_class << "\n";
  std::cout << "take_bar_class(7): " << take_bar_class << "\n";
  std::cout << "return_bar_class(7): " << return_bar_class << "\n";

  std::cout << "hello world\n";
  return 0;
}
