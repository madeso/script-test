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

template<class T>
static void do_test_push_ptr(lua_State *L) {
  T values[] = {T(), T(), T()};
  SLB3::Push<T*>(L, values);
  EXPECT_EQ(values, SLB3::Get<T*>(L,-1));
  EXPECT_EQ(values, SLB3::Get<const T*>(L,-1));
  EXPECT_EQ(values, SLB3::Get<const void*>(L,-1));
  EXPECT_EQ(values, SLB3::Get<void*>(L,-1));
  lua_pop(L,1);
}

template<class T>
static void do_test_push_const_ptr(lua_State *L) {
  T values[] = {T(), T(), T()};
  SLB3::Push<const T*>(L, values);
  EXPECT_EQ((T*)NULL, SLB3::Get<T*>(L,-1));
  EXPECT_EQ(NULL, SLB3::Get<void*>(L,-1));
  EXPECT_EQ(values, SLB3::Get<const T*>(L,-1));
  EXPECT_EQ(values, SLB3::Get<const void*>(L,-1));
  lua_pop(L,1);
}

TEST(PrimitivePointers, test) {
  Script s;
  do_test_push_ptr<float>(s);
  do_test_push_ptr<double>(s);
  do_test_push_ptr<int>(s);
  do_test_push_ptr<unsigned int>(s);
  do_test_push_ptr<short>(s);
  do_test_push_ptr<unsigned short>(s);
  do_test_push_ptr<long>(s);
  do_test_push_ptr<unsigned long>(s);

  EXPECT_EQ(0, lua_gettop(s));
}


TEST(PrimitivePointers, test_const) {
  Script s;
  do_test_push_const_ptr<float>(s);
  do_test_push_const_ptr<double>(s);
  do_test_push_const_ptr<int>(s);
  do_test_push_const_ptr<unsigned int>(s);
  do_test_push_const_ptr<short>(s);
  do_test_push_const_ptr<unsigned short>(s);
  do_test_push_const_ptr<long>(s);
  do_test_push_const_ptr<unsigned long>(s);

  EXPECT_EQ(0, lua_gettop(s));
}

TEST(PrimitivePointers, test_equality) {
  Script s;
  int v1[] = {1,2,3};
  int v2[] = {1,2,3};
  SLB3::Push(s, v1);
  SLB3::Push(s, v1);
  SLB3::Push(s, v2);
  EXPECT_EQ(1, lua_compare(s, 1, 2, LUA_OPEQ));
  EXPECT_EQ(0, lua_compare(s, 1, 3, LUA_OPEQ));
}

