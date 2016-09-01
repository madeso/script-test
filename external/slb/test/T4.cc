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
#include "T4.h"
#include "script.h"
#include "gtest/gtest.h"
#include <slb3/implementation.h>

SLB3_IMPLEMENTATION(T4Base, c) {
   c.set("get_a", &T4Base::get_a);
   c.set("set_a", &T4Base::get_a);
}
SLB3_IMPLEMENTATION(T4, c) {
   c.constructor();
   c.set("get_a", &T4::get_a);
   c.set("set_a", &T4::get_a);
   c.inherits<T4Base>();
}

static const T4Base* Invalid_t4b = NULL;
static const T4*     Invalid_t4  = NULL;

TEST(T4, build) {
  EXPECT_EQ(0, T4::Instance_count());
  {
    ref_ptr<T4> t;
    t.alloc();
    EXPECT_EQ(1, T4::Instance_count());
  }
  EXPECT_EQ(0, T4::Instance_count());
}

TEST(T4, build_p) {
  EXPECT_EQ(0, T4::Instance_count());
  {
    ref_ptr<T4> t4; t4.alloc();
    ref_ptr<T4Base> t = t4.get();
    EXPECT_EQ(1, T4::Instance_count());
  }
  EXPECT_EQ(0, T4::Instance_count());
}


TEST(T4, check_different_offsets) {
  EXPECT_EQ(0, T4::Instance_count());
  ref_ptr<T4_bis> t = new T4_bis();
  T4 *t4 = t.get();
  T4Base *t4b = t.get();
  EXPECT_NE(reinterpret_cast<void*>(t.get()), reinterpret_cast<void*>(t4));
  EXPECT_NE(reinterpret_cast<void*>(t.get()), reinterpret_cast<void*>(t4b));
  EXPECT_NE(reinterpret_cast<void*>(t4), reinterpret_cast<void*>(t4b));
  EXPECT_EQ(1, T4::Instance_count());
  t = NULL;
  EXPECT_EQ(0, T4::Instance_count());
}


TEST(T4, constructors_base_not_allowed) {
  Script s;
  SLB3::Register<T4Base>(s);
  SLB3::Register<T4>(s);
  EXPECT_FALSE(Execute(s, "a=T4Base()"));
}

TEST(T4, constructor_T4) {
  EXPECT_EQ(0, T4::Instance_count());
  Script s;
  SLB3::Register<T4Base>(s);
  SLB3::Register<T4>(s);
  EXPECT_TRUE(Execute(s, "a=T4()"));
  EXPECT_EQ(1, T4::Instance_count());
  s.finish();
  EXPECT_EQ(0, T4::Instance_count());
}

TEST(T4, constructor_T4_check_base) {
  EXPECT_EQ(0, T4::Instance_count());
  Script s;
  SLB3::Register<T4Base>(s);
  SLB3::Register<T4>(s);
  EXPECT_TRUE(Execute(s, "a=T4()"));
  EXPECT_EQ(1, T4::Instance_count());
  lua_getglobal(s, "a");
  T4 *t4 = SLB3::Value<T4*>::Get(s, -1);
  T4Base *t4b = SLB3::Value<T4Base*>::Get(s, -1);
  EXPECT_EQ(static_cast<T4Base*>(t4), t4b);
  EXPECT_EQ(dynamic_cast<T4*>(t4b), t4);
  EXPECT_NE((void*)(t4b), (void*)t4);
  s.finish();
  EXPECT_EQ(0, T4::Instance_count());
}


TEST(T4, hold_ref_from_parent) {
  EXPECT_EQ(0, T4::Instance_count());
  Script s;
  ref_ptr<T4> ref_t4 = new T4();
  SLB3::Value<T4Base*>::Push(s, ref_t4.get());
  EXPECT_EQ((unsigned)2, ref_t4->count());
  ref_t4 = NULL;
  s.finish();
  EXPECT_EQ(0, T4::Instance_count());
}

TEST(T4, check_raw_object) {
  EXPECT_EQ(0, T4::Instance_count());
  Script s;
  ref_ptr<T4> original = new T4();
  SLB3::Value<T4*>::Push(s, original.get());
  void *ptr = lua_touserdata(s,-1);
  SLB3::Object obj(ptr);
  // check
  EXPECT_EQ(obj.header->type, SLB3::kObject_Ptr);
  EXPECT_EQ(obj.unpack(), (void*)original.get());
  EXPECT_NE(obj.header->parents, (SLB3::internal::ParentRef*)NULL);
  // finish
  s.finish();
  EXPECT_EQ(1, T4::Instance_count());
  original = NULL;
  EXPECT_EQ(0, T4::Instance_count());
}

TEST(T4, checkConversion_to_parent) {
  EXPECT_EQ(0, T4::Instance_count());
  Script s;
  ref_ptr<T4> original = new T4();
  SLB3::Value<T4*>::Push(s, original.get());

  const T4Base *ptr = SLB3::Value<T4Base*>::Get(s, -1);
  EXPECT_NE(Invalid_t4b, ptr);
  EXPECT_EQ(ptr, SLB3::Value<const T4Base*>::Get(s, -1));
  EXPECT_EQ(ptr, &SLB3::Value<T4Base&>::Get(s, -1));
  EXPECT_EQ(ptr, &SLB3::Value<const T4Base&>::Get(s, -1));

  s.finish();
  EXPECT_EQ(1, T4::Instance_count());
  original = NULL;
  EXPECT_EQ(0, T4::Instance_count());
}

TEST(T4, checkConversion_from_parent) {
  EXPECT_EQ(0, T4::Instance_count());
  Script s;
  ref_ptr<T4> original = new T4();
  SLB3::Value<T4Base*>::Push(s, original.get());

  const T4Base *ptr = original.get();
  EXPECT_EQ(ptr, SLB3::Value<T4Base*>::Get(s, -1));
  EXPECT_EQ(ptr, SLB3::Value<const T4Base*>::Get(s, -1));
  EXPECT_EQ(ptr, &SLB3::Value<T4Base&>::Get(s, -1));
  EXPECT_EQ(ptr, &SLB3::Value<const T4Base&>::Get(s, -1));

  // Can't converty to child class
  EXPECT_EQ(Invalid_t4, SLB3::Value<T4*>::Get(s,-1));
  EXPECT_EQ(Invalid_t4, SLB3::Value<const T4*>::Get(s,-1));

  s.finish();
  original = NULL;
  EXPECT_EQ(0, T4::Instance_count());
}
