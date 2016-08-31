#include <iostream>
#include "shared.h"

#include "v8.h"

int main() {
  v8::HandleScope handle_scope;

  std::cout << "Hello world!\n";
  return 0;
}