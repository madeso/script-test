// Copyright (c) 2013, Jose L. Hidalgo Valiño <pplux@pplux.com>
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
// 
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
#include "script.h"
#include "gtest/gtest.h"
#include <SLB3/namespace.h>
#include <SLB3/extra/errorhandler.h>

int add_func(lua_State *L) {
  double a = lua_tonumber(L,1);
  double b = lua_tonumber(L,2);
  lua_pushnumber(L,a+b);
  return 1;
}

TEST(ErrorHandler, normal_call) {
  Script s;
  SLB3::extra::DefaultErrorHandler err;
  lua_pushcfunction(s, add_func);
  lua_pushnumber(s, 5);
  lua_pushnumber(s, 6);
  EXPECT_EQ(LUA_OK, err.call(s, 2, 1));
  EXPECT_EQ(1, lua_gettop(s));
  EXPECT_EQ(11.0, lua_tonumber(s, -1));
  EXPECT_STREQ("", err.last_error());
}

TEST(ErrorHandler, discard_result) {
  Script s;
  SLB3::extra::DefaultErrorHandler err;
  lua_pushcfunction(s, add_func);
  lua_pushnumber(s, 5);
  lua_pushnumber(s, 6);
  EXPECT_EQ(LUA_OK, err.call(s, 2, 0));
  EXPECT_EQ(0, lua_gettop(s));
  EXPECT_STREQ("", err.last_error());
}

TEST(ErrorHandler, fail_call_normal) {
  Script s;
  lua_pushnumber(s, 5);
  EXPECT_EQ(LUA_ERRRUN, lua_pcall(s, 0, 0, 0));
  EXPECT_EQ(1, lua_gettop(s));
  EXPECT_EQ(1, lua_isstring(s,-1));
  const char *error = lua_tostring(s, -1);
  EXPECT_STRNE("", error);
}

TEST(ErrorHandler, fail_call_handler) {
  Script s;
  SLB3::extra::DefaultErrorHandler err;
  lua_pushnumber(s, 5);
  EXPECT_EQ(LUA_ERRRUN, err.call(s, 0, 0));
  EXPECT_EQ(1, lua_gettop(s));
  EXPECT_EQ(1, lua_isstring(s,-1));
  EXPECT_STRNE("", err.last_error());
  //std::cout << "Error: " << err.last_error() << std::endl;
}

