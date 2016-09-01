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

void Script::init() {
  finish();
  L_ = luaL_newstate();
  luaL_openlibs(L_);
}

void Script::finish() {
  if (L_) lua_close(L_);
  L_ = NULL;
}

int Script::getInt(const char *name) {
  EXPECT_NE((lua_State*)0, L_);
  lua_getglobal(L_, name);
  int is_num = 1;
  int result = (int) lua_tointegerx(L_, -1, &is_num);
  lua_pop(L_,1);
  if (!is_num) {result = std::numeric_limits<int>::max();}
  return result;
}

const char* Script::getStr(const char *name) {
  const char *result = NULL;
  EXPECT_NE((lua_State*)0, L_);
  lua_getglobal(L_, name);
  if (lua_isstring(L_, -1)) result = lua_tostring(L_,-1);
  lua_pop(L_, 1);
  return result;
}

TEST(Script, ok_and_fail) {
 Script s;
 EXPECT_TRUE(Execute(s, "a=5"));
 EXPECT_FALSE(Execute(s, "invalid()"));
}


