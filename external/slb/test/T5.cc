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
#include "T5.h"
#include "script.h"
#include "gtest/gtest.h"
#include <slb3/implementation.h>


SLB3_IMPLEMENTATION(T5, cdec) {
  cdec
   .set("get_a", &T5::get_a)
   .set("set_a", &T5::set_a)
   .set("GetInstancePtr", T5::GetInstancePtr)
   .set("GetInstanceRef", T5::GetInstanceRef)
   .set("a_value_six", 6)
   .set("a_value_eight", 8)
   ;
}

TEST(T5, Basic) {
  T5 *t = T5::GetInstancePtr();
  T5::GetInstanceRef().set_a(1000);
  EXPECT_EQ(1000, t->get_a());
  EXPECT_EQ(t, T5::GetInstancePtr());
}

TEST(T5, no_constructor) {
  Script s;
  SLB3::Register<T5>(s);
  EXPECT_FALSE(Execute(s, "a=T5()"));

}

TEST(T5, calls) {
  Script s;
  SLB3::Register<T5>(s);
  T5::GetInstanceRef().set_a(15);
  EXPECT_TRUE(Execute(s, "a=T5.GetInstanceRef():get_a()"));
  EXPECT_TRUE(Execute(s, "b=T5.GetInstancePtr():get_a()"));
  EXPECT_EQ(15, s.getInt("a"));
  EXPECT_EQ(15, s.getInt("b"));
}

TEST(T5, values) {
  Script s;
  SLB3::Register<T5>(s);
  T5::GetInstanceRef().set_a(15);
  ASSERT_TRUE(Execute(s, "t5 = T5.GetInstancePtr()"));
  ASSERT_TRUE(Execute(s, "t5:set_a(t5.a_value_six)"));
  ASSERT_TRUE(Execute(s, "a=T5.GetInstanceRef():get_a()"));
  EXPECT_EQ(6, s.getInt("a"));
}
