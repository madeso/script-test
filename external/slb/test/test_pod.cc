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

struct Parameters {
  float a;
  int b;
  int c;
  T1 t1;
};

SLB3_POD_DECLARATION(Parameters)
SLB3_POD_IMPLEMENTATION(Parameters,
  SLB3_POD_MEMBER(a)
  SLB3_POD_MEMBER(b)
  SLB3_POD_MEMBER(c)
  SLB3_POD_MEMBER(t1)
)

bool test_param_a(const Parameters &param, float a) {
  return param.a == a;
}
bool test_param_b(const Parameters &param, int b) {
  return param.b == b;
}
bool test_param_c(const Parameters &param, int c) {
  return param.c == c;
}
bool test_param_t1a(const Parameters &param, float a) {
  return param.t1.get_a() == a;
}
bool test_param_t1b(const Parameters &param, int b) {
  return param.t1.get_b() == b;
}

void register_funcs(lua_State *s) {
   SLB3::Namespace(s, "test")
     .set("a", test_param_a)
     .set("b", test_param_b)
     .set("c", test_param_c)
     .set("t1a", test_param_t1a)
     .set("t1b", test_param_t1b)
  ;
}

TEST(pod, basic) {
   Script s;
   register_funcs(s);
   SLB3::Register<T1>(s);
   EXPECT_TRUE(Execute(s,"m={a=4.2;b=5;c=6;t1=testing.T1()}"));
   EXPECT_TRUE(Execute(s,"return test.a(m,4.2) or error()"));
   EXPECT_TRUE(Execute(s,"return test.b(m,5) or error()"));
   EXPECT_TRUE(Execute(s,"return test.c(m,6) or error()"));
   EXPECT_TRUE(Execute(s,"return test.t1a(m,1) or error()"));
   EXPECT_TRUE(Execute(s,"return test.t1b(m,2) or error()"));
}

