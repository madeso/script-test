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
#include "T2.h"
#include "script.h"
#include "gtest/gtest.h"
#include <slb3/implementation.h>

SLB3_IMPLEMENTATION(T2, c) {
   c.constructor<const char*>();
   c.set("get_t1_ptr", &T2::get_t1_ptr);
   c.set("get_t1_ref", &T2::get_t1_ref);
   c.set("get_t1_const_ptr", &T2::get_t1_const_ptr);
   c.set("get_t1_const_ref", &T2::get_t1_const_ref);
   c.set("get_t1_copy", &T2::get_t1_copy);
   c.set("set_txt", &T2::set_txt);
   c.set("txt", &T2::txt);
}

TEST(T2, Basic) {
  EXPECT_EQ(0, T2::Instance_count());
  EXPECT_EQ(0, T1::Instance_count());
  {
    Script s;
    T2 t("hi");
    SLB3::Value<T2*>::Push(s, &t);
    lua_setglobal(s, "t");
    EXPECT_TRUE(Execute(s, "result=t:txt()"));
    EXPECT_STREQ("hi", s.getStr("result"));
    EXPECT_EQ(1, T2::Instance_count());
    EXPECT_EQ(1, T1::Instance_count());
  }
  EXPECT_EQ(0, T2::Instance_count());
  EXPECT_EQ(0, T1::Instance_count());
}

struct T2_Test : public ::testing::Test {
  T2_Test() : t("TEST") {
    EXPECT_EQ(1, T2::Instance_count());
    EXPECT_EQ(1, T1::Instance_count());

    SLB3::Value<T2*>::Push(s, &t);
    lua_setglobal(s, "t");
  }
  ~T2_Test() {
    EXPECT_EQ(1, T2::Instance_count());
    EXPECT_EQ(1, T1::Instance_count());
  }
  T2 t;
  Script s;
};

TEST_F(T2_Test, fixture) {
  EXPECT_TRUE(Execute(s, "t:set_txt('boo')"));
  EXPECT_STREQ("boo", t.txt());
}


#define DO_TEST(name, code) \
TEST_F(T2_Test, name) {\
  EXPECT_TRUE(Execute(s, code));\
  lua_getglobal(s, "a");\
  EXPECT_EQ(t.get_t1_ptr(), SLB3::Value<T1*>::Get(s, -1));\
  EXPECT_EQ(t.get_t1_ptr(), SLB3::Value<const T1*>::Get(s, -1));\
  EXPECT_EQ(t.get_t1_ptr(), &SLB3::Value<T1&>::Get(s, -1));\
  EXPECT_EQ(t.get_t1_ptr(), &SLB3::Value<const T1&>::Get(s, -1));\
}\

DO_TEST(get_t1_ptr, "a=t:get_t1_ptr()")
DO_TEST(get_t1_ref, "a=t:get_t1_ref()")
#undef DO_TEST

#define DO_TEST(name, code) \
TEST_F(T2_Test, name) {\
  EXPECT_TRUE(Execute(s, code));\
  lua_getglobal(s, "a");\
  EXPECT_EQ((T1*)0L, SLB3::Value<T1*>::Get(s, -1));\
  EXPECT_EQ(t.get_t1_ptr(), SLB3::Value<const T1*>::Get(s, -1));\
  EXPECT_EQ(t.get_t1_ptr(), &SLB3::Value<const T1&>::Get(s, -1));\
}\

DO_TEST(get_t1_const_ptr, "a=t:get_t1_const_ptr()")
DO_TEST(get_t1_const_ref, "a=t:get_t1_const_ref()")
#undef DO_TEST


TEST_F(T2_Test, copy) {
  int count = T1::Instance_count();
  EXPECT_TRUE(Execute(s, "b,c=t:get_t1_copy(),t:get_t1_copy()"));
  EXPECT_EQ(count+2, T1::Instance_count());
  s.finish();
  EXPECT_EQ(count, T1::Instance_count());
}
