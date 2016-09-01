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
#include "T2.h"
#include "T3.h"
#include <SLB3/slb.h>
#include <SLB3/extra/package.h>

// This is a work in progress ---------------------------------------------------
#if SLB3_ENABLE_INFO_GENERATION
TEST(info, basic) {
   Script s;
   SLB3::Register<T1>(s);
   SLB3::Register<T2>(s);
   SLB3::Register<T3>(s);
   SLB3::extra::PreloadSLB3Package(s);

   EXPECT_TRUE(Execute(s,"slb3 = require('slb3')"));
   EXPECT_TRUE(Execute(s,"count = 0"));
   EXPECT_TRUE(Execute(s,"for k,v in pairs(slb3.classes()) do print(k); count = count+1 end"));
   EXPECT_TRUE(Execute(s,"return count == 3 or error()"));
}

TEST(info, t2_txt_method) {
  Script s;
  SLB3::Register<T2>(s);
  SLB3::extra::PreloadSLB3Package(s);

  EXPECT_TRUE(Execute(s,"slb3 = require('slb3')"));
  EXPECT_TRUE(Execute(s,"cls = slb3.classes().T2"));
  EXPECT_TRUE(Execute(s,"return cls ~= nil or error()"));
  EXPECT_TRUE(Execute(s,"func = cls.txt"));
  EXPECT_TRUE(Execute(s,"info = slb3.getinfo(func)"));
  EXPECT_TRUE(Execute(s,"return info.type == 'function' or error()"));
  EXPECT_TRUE(Execute(s,"return info.num_args == 1 or error()"));
  EXPECT_TRUE(Execute(s,"returned_type = info.type_returned()"));
  EXPECT_TRUE(Execute(s,"return returned_type ~= nil or error()"));
  EXPECT_TRUE(Execute(s,"return returned_type.name == 'const char *' or error()"));
}

TEST(info, t2_get_t1_const_ref) {
  Script s;
  SLB3::Register<T2>(s);
  SLB3::extra::PreloadSLB3Package(s);

  EXPECT_TRUE(Execute(s,"slb3 = require('slb3')"));
  EXPECT_TRUE(Execute(s,"cls = slb3.classes().T2"));
  EXPECT_TRUE(Execute(s,"return cls ~= nil or error()"));
  EXPECT_TRUE(Execute(s,"func = cls.get_t1_const_ref"));
  EXPECT_TRUE(Execute(s,"info = slb3.getinfo(func)"));
  EXPECT_TRUE(Execute(s,"return info.type == 'function' or error()"));
  EXPECT_TRUE(Execute(s,"return info.num_args == 1 or error()"));
  EXPECT_TRUE(Execute(s,"returned_type = info.type_returned()"));
  EXPECT_TRUE(Execute(s,"return returned_type ~= nil or error()"));
  EXPECT_TRUE(Execute(s,"return returned_type.name == 'const testing.T1 &' or error()"));
}

TEST(info, t2_get_t1_const_ptr) {
  Script s;
  SLB3::Register<T2>(s);
  SLB3::extra::PreloadSLB3Package(s);

  EXPECT_TRUE(Execute(s,"slb3 = require('slb3')"));
  EXPECT_TRUE(Execute(s,"cls = slb3.classes().T2"));
  EXPECT_TRUE(Execute(s,"return cls ~= nil or error()"));
  EXPECT_TRUE(Execute(s,"func = cls.get_t1_const_ptr"));
  EXPECT_TRUE(Execute(s,"info = slb3.getinfo(func)"));
  EXPECT_TRUE(Execute(s,"return info.type == 'function' or error()"));
  EXPECT_TRUE(Execute(s,"return info.num_args == 1 or error()"));
  EXPECT_TRUE(Execute(s,"returned_type = info.type_returned()"));
  EXPECT_TRUE(Execute(s,"return returned_type ~= nil or error()"));
  EXPECT_TRUE(Execute(s,"return returned_type.name == 'const testing.T1 *' or error()"));
  EXPECT_TRUE(Execute(s,"return info.args(0).name == 'const T2 *' or error()"));
}

TEST(info, t2_set_txt_method) {
  Script s;
  SLB3::Register<T2>(s);
  SLB3::extra::PreloadSLB3Package(s);

  EXPECT_TRUE(Execute(s,"slb3 = require('slb3')"));
  EXPECT_TRUE(Execute(s,"cls = slb3.classes().T2"));
  EXPECT_TRUE(Execute(s,"return cls ~= nil or error()"));
  EXPECT_TRUE(Execute(s,"func = cls.set_txt"));
  EXPECT_TRUE(Execute(s,"info = slb3.getinfo(func)"));
  EXPECT_TRUE(Execute(s,"return info.type == 'function' or error()"));
  EXPECT_TRUE(Execute(s,"return info.num_args == 2 or error()"));
  EXPECT_TRUE(Execute(s,"returned_type = info.type_returned()"));
  EXPECT_TRUE(Execute(s,"return returned_type == nil or error()"));
  EXPECT_TRUE(Execute(s,"arg1 = info.args(0) or error()"));
  EXPECT_TRUE(Execute(s,"arg2 = info.args(1) or error()"));
  EXPECT_TRUE(Execute(s,"return arg1.name == 'T2 *' or error()"));
  EXPECT_TRUE(Execute(s,"return arg2.name == 'const char *' or error()"));
}

#endif

