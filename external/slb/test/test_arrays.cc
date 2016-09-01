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
#include <SLB3/declaration.h>
#include <SLB3/implementation.h>

struct Vec2 {
  float x_,y_;
  Vec2() : x_(0.0f), y_(0.0f) {}
  Vec2(float a, float b) : x_(a), y_(b) {}
  float x() const { return x_; }
  float y() const { return y_; }
};

struct Box {
  typedef Vec2 Points[4];
  Points points_;
  Box() {}
  void init_default() {
    for(unsigned i = 0; i < 4; ++i) {
      points_[i].x_ = i*21.0f;
      points_[i].y_ = i*13.0f;
    }
  }
  void init(const Points &points ) {
    EXPECT_NE((const Vec2*)NULL, points);
    memcpy(points_, points, sizeof(points_));
  }
  const Vec2* get(unsigned pos) { return (pos < 4)? &points_[pos]:NULL; }
  Points &getAll() { return points_; }
  const Points &getAll_const() const { return points_; }
};

// Can't wrapp directly because the array of vec2
int w_Box_init(lua_State *L) {
  Box *b = SLB3::Get<Box*>(L,1);
  if (!b) return luaL_error(L, "Invalid object(Box) at #1");
  if (!SLB3::Value<Box::Points>::GetArray(L, 2, &b->points_)) {
    return luaL_error(L, "Can't get array of vec2 from #2");
  }
  return 0;
}

SLB3_DECLARATION(Vec2, "test.Vec2", policy::Default);
SLB3_DECLARATION(Box, "test.Box", policy::Default);

SLB3_IMPLEMENTATION(Vec2, C) { 
  C.constructor<float,float>()
   .set("x", &Vec2::x)
   .set("y", &Vec2::y)
;
}

SLB3_IMPLEMENTATION(Box, C) {
  C.constructor()
   .set("get", &Box::get)
   .set("init", w_Box_init)
   .set("init_default", &Box::init_default)
   .set("getAll", &Box::getAll)
   .set("getAll_const", &Box::getAll_const)
   ;
}

TEST(Arrays, pass_array) {
  Script s;
  SLB3::Register<Vec2>(s);
  SLB3::Register<Box>(s);
  EXPECT_TRUE(Execute(s,"v=test.Vec2"));
  EXPECT_TRUE(Execute(s,"b=test.Box()"));
  EXPECT_TRUE(Execute(s,"b:init{v(0,0),v(1,2),v(2,4),v(3,6)}"));
  lua_getglobal(s, "b");
  const Box *b = SLB3::Value<const Box*>::Get(s,-1);
  for(unsigned i = 0; i < 4; ++i) {
    EXPECT_EQ(i*1.0f, b->points_[i].x());
    EXPECT_EQ(i*2.0f, b->points_[i].y());
  }
}

 
