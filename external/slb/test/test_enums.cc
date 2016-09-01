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
#include "T1.h"
#include <SLB3/slb.h>

enum TestEnum {
  kValue1 = 1,
  kValue2 = 2,
  kValue3 = 3
};

SLB3_ENUM_DECLARATION(TestEnum);

TEST(enums, enum_check) {
   Script s;
   SLB3::Namespace(s, "test")
     .set("v_int", 5)
     .set("v_float", 5.6)
     .subtable("enum")
       .set("kValue1", kValue1)
       .set("kValue2", kValue2)
       .set("kValue3", kValue3)
       .close()
     .set("v_string", "hi there")
  ;
  EXPECT_EQ(0, lua_gettop(s));
  EXPECT_TRUE(Execute(s, "return test.v_int == 5 or error()"));
  EXPECT_TRUE(Execute(s, "return test.v_float == 5.6 or error()"));
  EXPECT_TRUE(Execute(s, "return test.v_string == 'hi there' or error()"));
  lua_settop(s,0);
  // kValue1
  EXPECT_TRUE(Execute(s, "a=test.enum.kValue1"));
  lua_getglobal(s, "a");
  EXPECT_EQ(kValue1,SLB3::Value<TestEnum>::Get(s, -1));
  lua_pop(s,1);
  EXPECT_EQ(0, lua_gettop(s));
  // kValue2
  EXPECT_TRUE(Execute(s, "a=test.enum.kValue2"));
  lua_getglobal(s, "a");
  EXPECT_EQ(kValue2,SLB3::Value<TestEnum>::Get(s, -1));
  lua_pop(s,1);
  EXPECT_EQ(0, lua_gettop(s));
  // kValue3
  EXPECT_TRUE(Execute(s, "a=test.enum.kValue3"));
  lua_getglobal(s, "a");
  EXPECT_EQ(kValue3,SLB3::Value<TestEnum>::Get(s, -1));
  lua_pop(s,1);
  EXPECT_EQ(0, lua_gettop(s));
}

TEST(enums, enum_to_integer) {
   Script s;
   SLB3::Push(s, kValue1);
   int v_i = SLB3::Get<int>(s, -1);
   unsigned int v_ui = SLB3::Get<unsigned int>(s, -1);
   EXPECT_EQ(v_i, 1);
   EXPECT_EQ(v_ui, (unsigned int)1);
   EXPECT_EQ(1, lua_gettop(s));
}

TEST(enums, enum_equality) {
   Script s;
   SLB3::Push(s, kValue1);
   SLB3::Push(s, kValue1);
   SLB3::Push(s, kValue2);
   EXPECT_EQ(1, lua_compare(s, 1, 2, LUA_OPEQ));
   EXPECT_EQ(0, lua_compare(s, 1, 3, LUA_OPEQ));
}

TestEnum func1() {
  return kValue1;
}

// This can issue a warning, but we need to test this
const TestEnum func2() {
  return kValue2;
}

TEST(enums, func_returns_enum) {
   Script s;
   SLB3::Namespace(s, "test")
     .subtable("enum")
       .set("kValue1", kValue1)
       .set("kValue2", kValue2)
       .set("kValue3", kValue3)
       .close()
     .set("func1", func1)
     .set("func2", func2)
   ;
   EXPECT_TRUE(Execute(s, "return test.enum.kValue1 == test.func1() or error('fail')"));
   EXPECT_TRUE(Execute(s, "return test.enum.kValue2 == test.func2() or error('fail')"));
}
