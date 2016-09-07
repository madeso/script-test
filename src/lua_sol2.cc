#include <iostream>
#include "shared.h"

#include "sol.hpp"

struct SolPerformance : LanguageTest<SolPerformance> {
  SolPerformance(sol::state& state) {
    rec = state["fib_rec"];
    loop = state["fib_loop"];
    str = state["string_test"];
  }

  int RunRec(int i) {
    return rec(i);
  }

  int RunLoop(int i) {
    return loop(i);
  }

  int RunStr(int i) {
    return str(i);
  }

  std::function<int (int)> rec;
  std::function<int (int)> loop;
  std::function<int (int)> str;
};

int main() {
  sol::state state;
  state.open_libraries(sol::lib::base, sol::lib::package, sol::lib::table, sol::lib::string);
  state.script_file("test.lua");

  std::cout << "\nAccessing variables\n";
  const int foo = state["foo"];
  const std::string bar = state["bar"][3];
  const std::string key = state["bar"]["key"];
  std::cout << "foo(4): " << foo << "\n";
  std::cout << "bar(hi): " << bar << "\n";
  std::cout << "key(there): " << key << "\n";

  std::cout << "\nCalling lua functions\n";

  state["ffoo"]();

  int result = state["add"](5, 2);
  std::cout << "result(7) " << result << "\n";
  int sum, difference;
  sol::tie(sum, difference) = state["sum_and_difference"](3, 1);
  std::cout << "sum(4): " << sum << "\ndiff(2): " << difference << "\n";

  result = state["mytable"]["foo"]();
  std::cout << "result(4): " << result << "\n";

  std::cout << "\nCalling C++ function from lua\n";
  state["test_func"] = &TestFunc;
  state.script("test_func()");

  std::cout << "\nClass test\n";
  state.new_usertype<Bar>("Bar",
                          sol::constructors<sol::types<int> >(),
                          "add_this", &Bar::AddThis);
  int use_bar_class = state["use_bar_class"]();
  int take_bar_class = state["take_bar_class"](Bar(5));
  Bar return_bar_class_bar = state["return_bar_class"]();
  int return_bar_class = return_bar_class_bar.AddThis(2);
  std::cout << "use_bar_class(7): " << use_bar_class << "\n";
  std::cout << "take_bar_class(7): " << take_bar_class << "\n";
  std::cout << "return_bar_class(7): " << return_bar_class << "\n";

  state["c_print"] = &Print;
  state.script("old_print(\"hello world\", 3, nil)");
  state.script("print(\"hello world\", 3, nil)");

  state["build_string"] = &BuildString;

  SolPerformance perf(state);
  perf.Run();

  return 0;
}
