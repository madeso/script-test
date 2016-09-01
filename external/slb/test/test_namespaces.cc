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
#include <SLB3/namespace.h>

TEST(Namespaces, basic_types) {
  Script s;
  SLB3::Namespace(s, "test")
    .set("v_int", 5)
    .set("v_float", 5.6)
    .set("v_string", "hi there")
 ;
 EXPECT_EQ(0, lua_gettop(s));
 EXPECT_TRUE(Execute(s, "return test.v_int == 5 or error()"));
 EXPECT_TRUE(Execute(s, "return test.v_float == 5.6 or error()"));
 EXPECT_TRUE(Execute(s, "return test.v_string == 'hi there' or error()"));
}

TEST(Namespaces, subtables) {
  Script s;
  SLB3::Namespace(s, "test")
    .set("v_int", 5)
    .set("v_float", 5.6)
    .set("v_string", "hi there")
    .subtable("nested")
      .set("a", 8)
      .set("b", 9)
        .subtable("nested2")
          .set("x", "really")
        .close()
      .set("c", 10)
    .close()
    .set("v_end", true)
 ;
 EXPECT_EQ(0, lua_gettop(s));
 EXPECT_TRUE(Execute(s, "return test.v_int == 5 or error()"));
 EXPECT_TRUE(Execute(s, "return test.v_float == 5.6 or error()"));
 EXPECT_TRUE(Execute(s, "return test.v_string == 'hi there' or error()"));
 EXPECT_TRUE(Execute(s, "return test.v_end or error()"));
 EXPECT_TRUE(Execute(s, "return test.nested.a == 8 or error()"));
 EXPECT_TRUE(Execute(s, "return test.nested.b == 9 or error()"));
 EXPECT_TRUE(Execute(s, "return test.nested.c == 10 or error()"));
 EXPECT_TRUE(Execute(s, "return test.nested.nested2.x == 'really' or error()"));
}

static int func_test_ADD(lua_State *L) {
  lua_pushnumber(L, lua_tonumber(L,1)+lua_tonumber(L,2));
  return 1;
}

TEST(Namespaces, lua_func) {
   Script s;
   SLB3::Namespace(s, "test.test_func")
     .set("arg_a", 5)
     .set("arg_b", 6)
   ;
   SLB3::Namespace(s, "test.test_func")
     .set("func", func_test_ADD)
   ;
  EXPECT_EQ(0, lua_gettop(s));
  EXPECT_TRUE(Execute(s, "a=test.test_func.arg_a"));
  EXPECT_TRUE(Execute(s, "b=test.test_func.arg_b"));
  EXPECT_TRUE(Execute(s, "func=test.test_func.func"));
  EXPECT_TRUE(Execute(s, "return (11 == func(a,b)) or error()"));
}

TEST(Namespaces, use_const_ref) {
  EXPECT_EQ(0, T1::Instance_count());
  Script s;
  T1 obj;
  SLB3::Namespace(s, "test.a.b.c")
    .set<const T1&>("t1", obj)
  ;
  EXPECT_TRUE(Execute(s, "a=test.a.b.c.t1"));
  lua_getglobal(s, "a");
  const T1* a = SLB3::Value<const T1*>::Get(s, -1);
  EXPECT_EQ(&obj, a);
  EXPECT_EQ((T1*)NULL, SLB3::Value<T1*>::Get(s,-1));
  EXPECT_EQ(1, T1::Instance_count());
  s.finish();
  EXPECT_EQ(1, T1::Instance_count());
}

TEST(Namespaces, use_copy_by_default) {
  EXPECT_EQ(0, T1::Instance_count());
  Script s;
  T1 obj;
  SLB3::Namespace(s, "test.a.b.c")
    .set("t1", obj)
  ;
  EXPECT_EQ(2, T1::Instance_count());
  EXPECT_TRUE(Execute(s, "a=test.a.b.c.t1"));
  lua_getglobal(s, "a");
  const T1* const_a = SLB3::Value<const T1*>::Get(s, -1);
  const T1* non_const_a = SLB3::Value<T1*>::Get(s, -1);
  EXPECT_NE(&obj, const_a);
  EXPECT_NE(&obj, non_const_a);
  EXPECT_EQ(const_a, non_const_a);
  s.finish();
  EXPECT_EQ(1, T1::Instance_count());
}

TEST(Namespaces, global_Env) {
  Script s;
  SLB3::Namespace(s, "_G")
    .set("a", 5)
  ;
  EXPECT_TRUE(Execute(s, "return a == 5 or error()"));
}

TEST(Namespace, global_env_empty) {
  Script s;
  SLB3::Namespace(s, "")
    .set("a", 5)
  ;
  EXPECT_TRUE(Execute(s, "return a==5 or error()"));
}

static float do_ADD(float a, float b) {
  return a+b;
}

TEST(Namespaces, wrapped_static_funcs) {
  Script s;
  SLB3::Namespace(s, "test")
    .set("ADD", do_ADD)
    .set("a", 32)
    .set("b", -16)
  ;
  EXPECT_TRUE(Execute(s, "return test.ADD(test.a, test.b) == 16 or error()"));
}

TEST(Namespace, methods) {
  Script s;
  T1 t1;
  t1.set_a(1024);
  SLB3::Namespace(s, "x")
    .set("method", &T1::get_a)
    .set<T1*>("instance", &t1)
  ;
  EXPECT_TRUE(Execute(s, "return x.method(x.instance) == 1024 or error()"));
}
