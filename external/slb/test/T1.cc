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
#include "T1.h"
#include "script.h"
#include "gtest/gtest.h"
#include <slb3/implementation.h>

SLB3_IMPLEMENTATION(T1, cdec) {
  cdec
   .constructor()
   .set("get_a", &T1::get_a)
   .set("get_b", &T1::get_b)
   .set("set_a", &T1::set_a)
   .set("set_b", &T1::set_b)
   .subtable("values")
      .set("val1", 1)
      .set("val2", 2)
      .close()
   ;
}

TEST(T1, Basic) {
  EXPECT_EQ(0, T1::Instance_count());
  T1 t;
  EXPECT_EQ(1, t.get_a());
  EXPECT_EQ(2, t.get_b());
  EXPECT_EQ(1, T1::Instance_count());
}

TEST(T1, Instances) {
  EXPECT_EQ(0, T1::Instance_count());
  T1 t[10];
  (void)t;
  EXPECT_EQ(10, T1::Instance_count());
}

TEST(T1, basic) {
  Script s; 
  EXPECT_TRUE(Execute(s,"a=5"));
  EXPECT_EQ(5, s.getInt("a"));
}

TEST(T1, structure) {
  Script s;
  SLB3::Register<T1>(s);
  EXPECT_TRUE(Execute(s, "return testing.T1 or error()"));
  EXPECT_TRUE(Execute(s, "return testing.T1.get_a or error()"));
  EXPECT_TRUE(Execute(s, "return testing.T1.get_b or error()"));
  EXPECT_TRUE(Execute(s, "m = debug.getmetatable(testing.T1); return m or error()"));
  EXPECT_TRUE(Execute(s, "return m.__call or error()"));
}

TEST(T1, structure_subtables) {
  Script s;
  SLB3::Register<T1>(s);
  EXPECT_TRUE(Execute(s, "return testing.T1.values.val1 == 1 or error()"));
  EXPECT_TRUE(Execute(s, "return testing.T1.values.val2 == 2 or error()"));
}

TEST(T1, push_ptr) {
  Script s; 
  EXPECT_EQ(0, T1::Instance_count());
  T1 t;
  SLB3::Value<T1*>::Push(s, &t);
  EXPECT_EQ(&t, SLB3::Value<T1*>::Get(s, -1));
  EXPECT_EQ((void*)&t, SLB3::Value<void*>::Get(s, -1));
  EXPECT_EQ((const void*)&t, SLB3::Value<const void*>::Get(s, -1));
  EXPECT_EQ(1, T1::Instance_count());
}

TEST(T1, push_const_ptr) {
  Script s; 
  EXPECT_EQ(0, T1::Instance_count());
  T1 t;
  SLB3::Value<const T1*>::Push(s, &t);
  EXPECT_EQ(&t, SLB3::Value<const T1*>::Get(s, -1));
  EXPECT_EQ(1, T1::Instance_count());
}

TEST(T1, check_const) {
  Script s; 
  EXPECT_EQ(0, T1::Instance_count());
  T1 t;
  SLB3::Value<const T1*>::Push(s, &t);
  EXPECT_EQ(NULL, SLB3::Value<T1*>::Get(s, -1));
  EXPECT_EQ(&t, SLB3::Value<const T1*>::Get(s, -1));
  EXPECT_EQ(NULL, SLB3::Value<void*>::Get(s, -1));
  EXPECT_EQ((const void*)&t, SLB3::Value<const void*>::Get(s, -1));
  EXPECT_EQ(1, T1::Instance_count());
}

TEST(T1, push_copy) {
  Script s; 
  EXPECT_EQ(0, T1::Instance_count());
  T1 t;
  SLB3::Value<T1>::Push(s, t);
  EXPECT_NE((T1*)0L, SLB3::Value<T1*>::Get(s, -1));
  EXPECT_NE(&t, SLB3::Value<const T1*>::Get(s, -1));
  EXPECT_NE(&t, SLB3::Value<T1*>::Get(s, -1));
  EXPECT_EQ(2, T1::Instance_count());
  s.finish();
  EXPECT_EQ(1, T1::Instance_count());
}

TEST(T1, construct_in_script) {
  Script s; 
  EXPECT_EQ(0, T1::Instance_count());
  SLB3::Register<T1>(s);
  EXPECT_TRUE(Execute(s,"a,b,c=testing.T1(),testing.T1(),testing.T1()"));
  EXPECT_EQ(3, T1::Instance_count());
  s.finish();
  EXPECT_EQ(0, T1::Instance_count());
}

TEST(T1, call_methods) {
  Script s; 
  EXPECT_EQ(0, T1::Instance_count());
  SLB3::Register<T1>(s);
  EXPECT_TRUE(Execute(s,"a=testing.T1()"));
  EXPECT_TRUE(Execute(s,"a:set_a(666)"));
  EXPECT_TRUE(Execute(s,"b=a:get_a()"));
  EXPECT_EQ(666, s.getInt("b"));
  EXPECT_EQ(1, T1::Instance_count());
  s.finish();
  EXPECT_EQ(0, T1::Instance_count());
}

TEST(T1, modify_passed_arg) {
  EXPECT_EQ(0, T1::Instance_count());
  Script s; 
  T1 t;
  SLB3::Value<T1*>::Push(s, &t);
  lua_setglobal(s, "a");
  EXPECT_TRUE(Execute(s,"a:set_b(777)"));
  EXPECT_EQ(777,t.get_b());
  EXPECT_EQ(1, T1::Instance_count());
}

TEST(T1, equality) {
  Script s; 
  T1 t1;
  T1 t2;
  SLB3::Push(s, &t1);
  SLB3::Push(s, &t1);
  SLB3::Push(s, &t2);
  EXPECT_EQ(1, lua_compare(s, 1, 2, LUA_OPEQ));
  EXPECT_EQ(0, lua_compare(s, 1, 3, LUA_OPEQ));
}

TEST(T1, null_test_push) {
  Script s; 
  T1* t1 = NULL;
  SLB3::Push(s, t1);
  lua_setglobal(s,"a");
  EXPECT_TRUE(Execute(s,"return a==nil or error('a is not nil')"));
}

TEST(T1, null_test_get) {
  Script s; 
  lua_pushnil(s);
  EXPECT_EQ((const T1*)NULL, SLB3::Get<T1*>(s,-1));
  EXPECT_EQ((const T1*)NULL, SLB3::Get<const T1*>(s,-1));
}

