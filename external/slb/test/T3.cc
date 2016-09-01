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
#include "T3.h"
#include "script.h"
#include "gtest/gtest.h"
#include <slb3/implementation.h>

SLB3_IMPLEMENTATION(T3, c) {
   c.constructor();
   c.set("get_a", &T3::get_a);
   c.set("set_a", &T3::get_a);
}

TEST(T3, build) {
  EXPECT_EQ(0, T3::Instance_count());
  {
    ref_ptr<T3> t3;
    t3.alloc();
    EXPECT_EQ(1, T3::Instance_count());
  }
  EXPECT_EQ(0, T3::Instance_count());
}


TEST(T3, build_script) {
  Script s;
  EXPECT_EQ(0, T3::Instance_count());
  SLB3::Register<T3>(s);
  EXPECT_TRUE(Execute(s, "t = T3()"));
  s.finish();
  EXPECT_EQ(0, T3::Instance_count());
}

TEST(T3, hold_ref) {
  Script s;
  ref_ptr<T3> ref_t3;
  // create a ref inside the script
  EXPECT_EQ(0, T3::Instance_count());
  SLB3::Register<T3>(s);
  EXPECT_TRUE(Execute(s, "t = T3()"));
  // recover the object and hold it externally
  lua_getglobal(s, "t");
  ref_t3 = SLB3::Value<T3*>::Get(s, -1);
  EXPECT_NE((T3*)NULL, ref_t3.get());
  EXPECT_EQ((unsigned)2, ref_t3->count());
  // close lua state and check object's presence
  s.finish();
  EXPECT_EQ((unsigned)1, ref_t3->count());
  EXPECT_EQ(1, T3::Instance_count());
  // finalize
  ref_t3 = NULL;
  EXPECT_EQ(0, T3::Instance_count());
}

TEST(T3, hold_ref_reversed) {
  EXPECT_EQ(0, T3::Instance_count());
  Script s;
  ref_ptr<T3> ref_t3 = new (malloc(sizeof(T3))) T3();
  const T3* t3 = ref_t3.get();
  EXPECT_EQ(1, T3::Instance_count());
  // Push onto lua stack references of ref_t3, check count
  SLB3::Value<T3*>::Push(s, ref_t3.get());
  EXPECT_EQ((unsigned)2, t3->count());
  SLB3::Value<T3&>::Push(s, *ref_t3.get());
  EXPECT_EQ((unsigned)3, t3->count());
  // this two doesn't add to the count of refernces
  SLB3::Value<const T3*>::Push(s, ref_t3.get());
  EXPECT_EQ((unsigned)3, t3->count());
  SLB3::Value<const T3&>::Push(s, *ref_t3.get());
  EXPECT_EQ((unsigned)3, t3->count());
  // Start removing references ----------------
  ref_t3 = NULL;
  EXPECT_EQ((unsigned)2, t3->count());
  // pop values from stack
  lua_pop(s,2); // nothing should change, only const value removed
  lua_gc(s, LUA_GCCOLLECT, 0);
  EXPECT_EQ((unsigned)2, t3->count());
  // pop one element (ref)
  lua_pop(s,1);
  lua_gc(s, LUA_GCCOLLECT, 0);
  EXPECT_EQ((unsigned)1, t3->count());
  // pop last element (ptr), t3 becomes invalid
  lua_pop(s,1);
  lua_gc(s, LUA_GCCOLLECT, 0);
  // check number of instances of T3
  EXPECT_EQ(0, T3::Instance_count());
  EXPECT_EQ(0, lua_gettop(s));
}

