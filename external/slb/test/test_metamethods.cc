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
#include <SLB3/value.h>
#include <SLB3/implementation.h>
#include <SLB3/declaration.h>


struct Vec3 {
  float x,y,z;
  Vec3(float a, float b, float c) : x(a), y(b), z(c) {
  }
};

int Vec3Get(lua_State *L) {
  const Vec3 *v = SLB3::Get<const Vec3*>(L,1);
  if (!v) return luaL_error(L, "Invalid Vec3 object at 1");
  if (lua_isnumber(L,2)) {
    int index = (int)lua_tonumber(L,2);
    switch(index) {
      case 1 : SLB3::Push(L, v->x); return 1;
      case 2 : SLB3::Push(L, v->y); return 1;
      case 3 : SLB3::Push(L, v->z); return 1;
    }
    luaL_error(L, "Invalid index %d", index);
  }
  if (lua_isstring(L,2)) {
    const char *key = lua_tostring(L,2);
    if (key[1] == '\0') {
      switch(key[0]) {
        case 'x': SLB3::Push(L,v->x); return 1;
        case 'y': SLB3::Push(L,v->y); return 1;
        case 'z': SLB3::Push(L,v->z); return 1;
      }
    }
    return luaL_error(L, "Invalid key %s", key);
  }
  return luaL_error(L, "Expect second argument string or index");
}

Vec3 Vec3__add(const Vec3 &a, const Vec3 &b) {
  return Vec3(a.x+b.x, a.y+b.y, a.z+b.z);
}

Vec3 Vec3__sub(const Vec3 &a, const Vec3 &b) {
  return Vec3(a.x-b.x, a.y-b.y, a.z-b.z);
}

SLB3_DECLARATION(Vec3, "test.Vec3", policy::Default);

SLB3_IMPLEMENTATION(Vec3, C) {
  C.constructor<float,float,float>()
   .set("Get", Vec3Get)
   .setMetamethod("__add", Vec3__add)
   .setMetamethod("__sub", Vec3__sub)
   ;
}

TEST(metamethods, basic) {
  Script s;
  SLB3::Register<Vec3>(s);
  EXPECT_TRUE(Execute(s, "a = test.Vec3(1,2,3)"));
  EXPECT_TRUE(Execute(s, "return a:Get(1) == 1 or error()"));
  EXPECT_TRUE(Execute(s, "return a:Get(2) == 2 or error()"));
  EXPECT_TRUE(Execute(s, "return a:Get(3) == 3 or error()"));
  EXPECT_TRUE(Execute(s, "return a:Get('x') == 1 or error()"));
  EXPECT_TRUE(Execute(s, "return a:Get('y') == 2 or error()"));
  EXPECT_TRUE(Execute(s, "return a:Get('z') == 3 or error()"));
}

TEST(metamethods, add) {
  Script s;
  SLB3::Register<Vec3>(s);
  EXPECT_TRUE(Execute(s, "a = test.Vec3(1,2,3)"));
  EXPECT_TRUE(Execute(s, "b = test.Vec3(1,2,3)"));
  EXPECT_TRUE(Execute(s, "r = a+b"));
  EXPECT_TRUE(Execute(s, "return r:Get(1) == 2 or error()"));
  EXPECT_TRUE(Execute(s, "return r:Get(2) == 4 or error()"));
  EXPECT_TRUE(Execute(s, "return r:Get(3) == 6 or error()"));
}

TEST(metamethods, sub) {
  Script s;
  SLB3::Register<Vec3>(s);
  EXPECT_TRUE(Execute(s, "a = test.Vec3(1,2,3)"));
  EXPECT_TRUE(Execute(s, "b = test.Vec3(5,8,11)"));
  EXPECT_TRUE(Execute(s, "r = a-b"));
  EXPECT_TRUE(Execute(s, "return r:Get(1) == -4 or error()"));
  EXPECT_TRUE(Execute(s, "return r:Get(2) == -6 or error()"));
  EXPECT_TRUE(Execute(s, "return r:Get(3) == -8 or error()"));
}
