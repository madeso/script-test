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
#include <SLB3/declaration.h>
#include <SLB3/implementation.h>

// Tests from oolua webpage (class exchangign)
// https://code.google.com/p/oolua/wiki/Speed_comparisons
class ProfileBase
{
  public:
    ProfileBase():_i(0){}
    virtual ~ProfileBase(){}
    void increment_a_base(ProfileBase* base)
    {
      ++base->_i;
    }
    virtual void virtual_func()
    {
      ++_i;
    }
    virtual void pure_virtual_func() = 0;
  private:
    int _i;
};

class ProfileAnotherBase
{
  public:
    virtual ~ProfileAnotherBase(){}
};

class ProfileDerived : public ProfileBase
{
  public:
    virtual ~ProfileDerived(){}
    virtual void pure_virtual_func()
    {
      ++_i;
    }
  private:
    int _i;
};

class ProfileMultiBases : public ProfileDerived, public ProfileAnotherBase
{
  public:
    void virtual_func()
    {
      ++_i;
    }
  private:
    int _i;
};

SLB3_DECLARATION(ProfileBase, "ProfileBase", policy::Default)
SLB3_DECLARATION(ProfileAnotherBase, "ProfileAnotherBase", policy::Default)
SLB3_DECLARATION(ProfileDerived, "ProfileDerived", policy::Default)
SLB3_DECLARATION(ProfileMultiBases, "ProfileMultiBases", policy::Default)
         
         
SLB3_IMPLEMENTATION(ProfileBase, C) {
  C.set("increment_a_base",&ProfileBase::increment_a_base);
  C.set("virtual_func", &ProfileBase::virtual_func);
  C.set("pure_virtual_func", &ProfileBase::pure_virtual_func);
}
SLB3_IMPLEMENTATION(ProfileAnotherBase, C) {
    C.constructor();
}
SLB3_IMPLEMENTATION(ProfileDerived, C) {
  C.constructor();
  C.inherits<ProfileBase>();
}
SLB3_IMPLEMENTATION(ProfileMultiBases, C) {
  C.constructor();
  C.inherits<ProfileBase>();
  C.inherits<ProfileDerived>();
  C.inherits<ProfileAnotherBase>();
}

TEST(oolua_tests, class_exchangign_00) {
   Script s;
   SLB3::Register<ProfileDerived>(s);

   EXPECT_TRUE(Execute(s, "a = ProfileDerived()"));
   // Check ProfileDerived
   lua_getglobal(s, "a");
   EXPECT_NE((ProfileBase*)NULL, SLB3::Get<ProfileBase*>(s,-1));
   EXPECT_EQ((ProfileAnotherBase*)NULL, SLB3::Get<ProfileAnotherBase*>(s,-1));
}

TEST(oolua_tests, class_exchangign_01) {
   Script s;
   SLB3::Register<ProfileMultiBases>(s);

   EXPECT_TRUE(Execute(s, "b = ProfileMultiBases()"));
   // Check ProfileMultiBases
   lua_getglobal(s, "b");
   EXPECT_NE((ProfileBase*)NULL, SLB3::Get<ProfileBase*>(s,-1));
   EXPECT_NE((ProfileAnotherBase*)NULL, SLB3::Get<ProfileAnotherBase*>(s,-1));
}

TEST(oolua_tests, class_exchangign_1) {
   Script s;
   SLB3::Register<ProfileBase>(s);
   SLB3::Register<ProfileAnotherBase>(s);
   SLB3::Register<ProfileDerived>(s);
   SLB3::Register<ProfileMultiBases>(s);

   EXPECT_TRUE(Execute(s, 
"     local N = 10"
"     local ave = 0"
"     local times = 10000"

"     for i = 0, N do"
"       local obj = ProfileDerived()"
"       local increment_me = ProfileDerived()"

"       local t0 = os.clock()"
"       for i=1,times do"
"         obj:increment_a_base(increment_me)"
"       end"
"       local dt = os.clock()-t0"
"       if i~=0 then"
"         ave = ave + dt"
"       end"
"     end"
"     print('passing derived to a function that wants a base  (average elapsed time):',ave/N)"
));
}

TEST(oolua_tests, class_exchangign_2) {
   Script s;
   SLB3::Register<ProfileBase>(s);
   SLB3::Register<ProfileAnotherBase>(s);
   SLB3::Register<ProfileDerived>(s);
   SLB3::Register<ProfileMultiBases>(s);

   EXPECT_TRUE(Execute(s, 
"    local N = 10"
"    local ave = 0"
"    local times = 10000"
"    for i = 0, N do"
"      local obj = ProfileDerived()"
"      local increment_me = ProfileMultiBases()"

"      local t0 = os.clock()"
"      for i=1,times do"
"        obj:increment_a_base(increment_me)"
"      end"
"      local dt = os.clock()-t0"
"      if i~=0 then"
"        ave = ave + dt"
"      end"
"    end"
"    print('passing derived with multiple bases to a function that wants a base  (average elapsed time):',ave/N)"
));
}

