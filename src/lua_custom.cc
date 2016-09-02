#include <iostream>
#include "shared.h"

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class Selector {
 public:
  Selector(lua_State* state) : state_(state) {}

  ~Selector() {
    if( state_ != nullptr) {
      lua_remove(state_, -1);
    }
  }

  Selector operator[](const std::string& name) {
    lua_getfield(state_, -1, name.c_str());
    lua_remove(state_, -2); // remove parent
    lua_State* old_state = state_;
    state_ = nullptr;
    return Selector(old_state);
  }

  Selector operator[](const char* const name) {
    return (*this)[std::string(name)];
  }

  Selector operator[](int index) {
    lua_geti(state_, -1, index);
    lua_remove(state_, -2); // remove parent
    lua_State* old_state = state_;
    state_ = nullptr;
    return Selector(old_state);
  }

  operator bool() {
    return static_cast<bool>(lua_toboolean(state_, -1));
  }

  operator float() {
    if(!lua_isnumber(state_, -1)) {
      // "Not a number"
      return -42.f;
    }
    return static_cast<float>(lua_tonumber(state_, -1));
  }

  operator int() {
    if(!lua_isnumber(state_, -1)) {
      // "Not a number"
      return 42;
    }
    return static_cast<int>(lua_tonumber(state_, -1));
  }

  operator std::string() {
    std::string s = "null";
    if(!lua_isstring(state_, -1)) {
      // not a string
      return "invalid";
    }

    return lua_tostring(state_, -1);
  }

 private:
  lua_State* state_;
};

class Script {
 public:
  Script() : state_(luaL_newstate()) {
  }

  bool Load(const std::string& file) {
    if (luaL_loadfile(state_, file.c_str()) || lua_pcall(state_, 0, 0, 0)) {
      return false;
    }
    return true;
  }

  Selector operator[](const std::string& var) {
    const int type = lua_getglobal(state_, var.c_str());
    return Selector(state_);
  }

  ~Script() {
    if(state_ != nullptr) {
      lua_close(state_);
      state_ = nullptr;
    }
  }

 private:
  lua_State* state_;
};


int main() {
  Script state;
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

  std::cout << "Hello world!\n";
  return 0;
}
