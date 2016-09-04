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

  std::cout << "\nCalling lua functions\n";

  auto ff = chai.eval<std::function<void (void)>>("ffoo");
  ff();

  auto fadd = chai.eval<std::function<int (int, int)>>("add");
  auto a = fadd(3, 2);
  int result = fadd(5, 2);
  std::cout << "result(7) " << result << "\n";

  std::cout << "hello world\n";
  return 0;
}
