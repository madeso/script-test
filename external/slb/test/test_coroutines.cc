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
#include <vector>
#include "script.h"
#include "T1.h"
#include "gtest/gtest.h"
#include <SLB3/namespace.h>
#include <SLB3/extra/coroutinemanager.h>

using namespace SLB3::extra;

class CoroutineManagerTestBase : public CoroutineManager {
public:
  int coroutines_created;
  int coroutines_suspended;
  int coroutines_signaled;
  int coroutines_finished;
  int coroutines_aborted;
  int coroutines_error;
  bool premature_lua_close;
protected:
  CoroutineManagerTestBase();
  ~CoroutineManagerTestBase() {}
  virtual void event_implementation(
      const CoroutineManager::Event,
      const CoroutineManager::Thread thread,
      const char *error);
};

class SimpleYieldCM : public CoroutineManagerTestBase {
public:
  SimpleYieldCM() {}
  ~SimpleYieldCM() {}
protected:
  WaitForResult waitFor_implementation(Thread t, int id) {
    (void)t; (void)id;
    return kW_WaitUntilNextUpdate;
  }
};

class SimpleSignalCM : public CoroutineManagerTestBase {
public:
  SimpleSignalCM() {}
  ~SimpleSignalCM() {}

  void signal_last() {
    EXPECT_EQ(true, signalCoroutine(waiting_.back()));
    waiting_.pop_back();
  }
  void abort_last() {
    EXPECT_EQ(true, abortCoroutine(waiting_.back()));
    waiting_.pop_back();
  }
  size_t waiting_size() const { return waiting_.size(); }

protected:
  typedef std::vector<CoroutineManager::Thread> T_ThreadsVector;
  WaitForResult waitFor_implementation(Thread t, int) {
    waiting_.push_back(t);
    return CoroutineManager::kW_WaitForSignal;
  }
  T_ThreadsVector waiting_;
};

TEST(Coroutines, basic) {
  Script s;
  SimpleYieldCM cm;
  s.init();
  cm.init(s);
  EXPECT_EQ(0, lua_gettop(s));
  lua_getglobal(s, "waitFor");
  EXPECT_EQ(true, lua_isfunction(s,-1));
  lua_getglobal(s, "async");
  EXPECT_EQ(true, lua_isfunction(s,-1));
  lua_pop(s,2);
  cm.finish();
  EXPECT_EQ(0, lua_gettop(s));
  EXPECT_EQ(0, cm.coroutines_created);
  EXPECT_EQ(0, cm.coroutines_suspended);
  EXPECT_EQ(0, cm.coroutines_signaled);
  EXPECT_EQ(0, cm.coroutines_finished);
  EXPECT_EQ(0, cm.coroutines_aborted);
  EXPECT_EQ(0, cm.coroutines_error);
  EXPECT_EQ(false, cm.premature_lua_close);
}

TEST(Coroutines, arguments) {
  Script s;
  SimpleYieldCM cm;
  s.init();
  cm.init(s);
  EXPECT_TRUE(Execute(s, "total=0"));
  EXPECT_TRUE(Execute(s, "async(function(a,b,c,d) total=a+b+c+d end, 1, 2, 3, 4)"));
  cm.update();
  EXPECT_EQ(10, s.getInt("total"));
}

TEST(Coroutines, arguments_extra) {
  Script s;
  SimpleYieldCM cm;
  s.init();
  cm.init(s);
  EXPECT_TRUE(Execute(s, "total=0"));
  EXPECT_TRUE(Execute(s, "async(function(a,b,c,d) total=a+b+c+d end, 1, 2, 3, 4,5,6,7)"));
  cm.update();
  EXPECT_EQ(10, s.getInt("total"));
}

TEST(Coroutines, arguments_async_inside_async) {
  Script s;
  SimpleYieldCM cm;
  s.init();
  cm.init(s);
  EXPECT_TRUE(Execute(s, "total=0"));
  EXPECT_TRUE(Execute(s, "function test_2(a,b) total = a+b end"));
  EXPECT_TRUE(Execute(s, "function test(a,b)async(test_2, a+b, 3,400,500) end"));
  EXPECT_TRUE(Execute(s, "async(test, 1,2,100,200)"));
  cm.update();
  cm.update();
  EXPECT_EQ(6, s.getInt("total"));
}

TEST(Coroutines, premature_flag_true) {
  Script s;
  SimpleYieldCM cm;
  s.init();
  cm.init(s);
  EXPECT_EQ(0, lua_gettop(s));
  s.finish();
  EXPECT_EQ((lua_State*)NULL, (lua_State*)s);
  cm.finish();
  EXPECT_EQ(true, cm.premature_lua_close);
}

TEST(Coroutines, async_without_wait) {
  Script s;
  SimpleYieldCM cm;
  s.init();
  cm.init(s);
  EXPECT_TRUE(Execute(s, "a= 0"));
  EXPECT_TRUE(Execute(s, "async(function() a=1 end)"));
  EXPECT_EQ(1, cm.coroutines_created);
  EXPECT_EQ(0, s.getInt("a"));
  EXPECT_EQ(0, cm.coroutines_suspended);
  cm.update();
  EXPECT_EQ(1, s.getInt("a"));
  EXPECT_EQ(1, cm.coroutines_created);
  EXPECT_EQ(0, cm.coroutines_suspended);
  EXPECT_EQ(0, cm.coroutines_signaled);
  EXPECT_EQ(1, cm.coroutines_finished);
  EXPECT_EQ(0, cm.coroutines_aborted);
  EXPECT_EQ(0, cm.coroutines_error);
  EXPECT_EQ(false, cm.premature_lua_close);
  cm.finish();
  EXPECT_EQ(0, lua_gettop(s));
  s.finish();
}

TEST(Coroutines, simple_yield_1) {
  Script s;
  SimpleYieldCM cm;
  s.init();
  cm.init(s);
  EXPECT_TRUE(Execute(s, "a= 0"));
  EXPECT_TRUE(Execute(s, "async(function() a=1 waitFor() a=2 end)"));
  EXPECT_EQ(1, cm.coroutines_created);
  EXPECT_EQ(0, s.getInt("a"));
  EXPECT_EQ(0, cm.coroutines_suspended);
  // First exec
  cm.update();
  EXPECT_EQ(1, s.getInt("a"));
  // Second round
  cm.update();
  EXPECT_EQ(2, s.getInt("a"));
  cm.finish();
  EXPECT_EQ(1, cm.coroutines_created);
  EXPECT_EQ(0, cm.coroutines_suspended);
  EXPECT_EQ(0, cm.coroutines_signaled);
  EXPECT_EQ(1, cm.coroutines_finished);
  EXPECT_EQ(0, cm.coroutines_error);
  EXPECT_EQ(0, cm.coroutines_aborted);
  EXPECT_EQ(false, cm.premature_lua_close);
  EXPECT_EQ(0, lua_gettop(s));
  s.finish();
}

TEST(Coroutines, simple_yield_2) {
  Script s;
  SimpleYieldCM cm;
  s.init();
  cm.init(s);
  EXPECT_TRUE(Execute(s, "a,b = 0,0"));
  EXPECT_TRUE(Execute(s, "async(function() a=1 waitFor() a=2 end)"));
  EXPECT_EQ(1, cm.coroutines_created);
  EXPECT_TRUE(Execute(s, "async(function() b=1 waitFor() b=2 end)"));
  EXPECT_EQ(2, cm.coroutines_created);
  EXPECT_EQ(0, cm.coroutines_suspended);
  // Start
  EXPECT_EQ(0, s.getInt("a"));
  EXPECT_EQ(0, s.getInt("b"));
  EXPECT_EQ(2u, cm.total_coroutines());
  EXPECT_EQ(2u, cm.active_coroutines());
  // First exec
  cm.update();
  EXPECT_EQ(1, s.getInt("a"));
  EXPECT_EQ(1, s.getInt("b"));
  EXPECT_EQ(2u, cm.total_coroutines());
  EXPECT_EQ(2u, cm.active_coroutines());
  // Second round
  cm.update();
  EXPECT_EQ(2, s.getInt("a"));
  EXPECT_EQ(2, s.getInt("b"));
  EXPECT_EQ(0u, cm.total_coroutines());
  EXPECT_EQ(0u, cm.active_coroutines());
  // Finish:
  cm.finish();
  EXPECT_EQ(2, cm.coroutines_created);
  EXPECT_EQ(0, cm.coroutines_suspended);
  EXPECT_EQ(0, cm.coroutines_signaled);
  EXPECT_EQ(2, cm.coroutines_finished);
  EXPECT_EQ(0, cm.coroutines_aborted);
  EXPECT_EQ(0, cm.coroutines_error);
  EXPECT_EQ(false, cm.premature_lua_close);
  EXPECT_EQ(0, lua_gettop(s));
  s.finish();
}

TEST(Coroutines, waitfor_signal_abort) {
  Script s;
  SimpleSignalCM cm;
  s.init();
  cm.init(s);
  EXPECT_TRUE(Execute(s, "a,b,c,d = 0,0,0,0"));
  EXPECT_TRUE(Execute(s, "function test() waitFor() end"));
  EXPECT_TRUE(Execute(s, "async(test)"));
  EXPECT_TRUE(Execute(s, "async(test)"));
  EXPECT_TRUE(Execute(s, "async(test)"));
  EXPECT_TRUE(Execute(s, "async(test)"));
  cm.update();
  EXPECT_EQ(4u, cm.waiting_size());
  cm.finish();
  EXPECT_EQ(4, cm.coroutines_created);
  EXPECT_EQ(4, cm.coroutines_suspended);
  EXPECT_EQ(0, cm.coroutines_signaled);
  EXPECT_EQ(0, cm.coroutines_finished);
  EXPECT_EQ(4, cm.coroutines_aborted);
  EXPECT_EQ(0, cm.coroutines_error);
  EXPECT_EQ(false, cm.premature_lua_close);
  EXPECT_EQ(0, lua_gettop(s));
  s.finish();
}

TEST(Coroutines, waitfor_signal_test1) {
  Script s;
  SimpleSignalCM cm;

  s.init();
  cm.init(s);
  EXPECT_TRUE(Execute(s, "a,b,c,d = 0,0,0,0"));
  EXPECT_TRUE(Execute(s, "function test(v, add) _G[v]=_G[v]+add waitFor() _G[v]=_G[v]+add end"));
  // Start 2 threads
  EXPECT_TRUE(Execute(s, "async(test, 'a', 1)"));
  EXPECT_TRUE(Execute(s, "async(test, 'b', 2)"));
  EXPECT_EQ(2, cm.coroutines_created);
  EXPECT_EQ(0, cm.coroutines_suspended);
  EXPECT_EQ(0u, cm.waiting_size());
  EXPECT_EQ(2u, cm.total_coroutines());
  EXPECT_EQ(2u, cm.active_coroutines());
  // Execute coroutines, and add two more
  cm.update();
  EXPECT_EQ(2, cm.coroutines_created);
  EXPECT_EQ(2, cm.coroutines_suspended);
  EXPECT_EQ(2u, cm.waiting_size());
  EXPECT_EQ(2u, cm.total_coroutines());
  EXPECT_EQ(0u, cm.active_coroutines());
  EXPECT_EQ(1, s.getInt("a"));
  EXPECT_EQ(2, s.getInt("b"));
  // add two
  EXPECT_TRUE(Execute(s, "async(test, 'c', 3)"));
  EXPECT_TRUE(Execute(s, "async(test, 'd', 4)"));
  EXPECT_EQ(4, cm.coroutines_created);
  EXPECT_EQ(2, cm.coroutines_suspended);
  EXPECT_EQ(2u, cm.waiting_size());
  EXPECT_EQ(4u, cm.total_coroutines());
  EXPECT_EQ(2u, cm.active_coroutines());
  // And update
  cm.update();
  EXPECT_EQ(4, cm.coroutines_created);
  EXPECT_EQ(4, cm.coroutines_suspended);
  EXPECT_EQ(4u, cm.waiting_size());
  EXPECT_EQ(4u, cm.total_coroutines());
  EXPECT_EQ(0u, cm.active_coroutines());
  EXPECT_EQ(1, s.getInt("a"));
  EXPECT_EQ(2, s.getInt("b"));
  EXPECT_EQ(3, s.getInt("c"));
  EXPECT_EQ(4, s.getInt("d"));
  // And update several times (again... nothing should change)
  for(unsigned i = 0; i < 10; i++) {
    cm.update();
    EXPECT_EQ(4, cm.coroutines_created);
    EXPECT_EQ(4, cm.coroutines_suspended);
    EXPECT_EQ(0, cm.coroutines_signaled);
    EXPECT_EQ(4u, cm.waiting_size());
    EXPECT_EQ(4u, cm.total_coroutines());
    EXPECT_EQ(0u, cm.active_coroutines());
    EXPECT_EQ(1, s.getInt("a"));
    EXPECT_EQ(2, s.getInt("b"));
    EXPECT_EQ(3, s.getInt("c"));
    EXPECT_EQ(4, s.getInt("d"));
  }
  // Now signal last threads to wake up
  // remember to remove them from the list
  cm.signal_last();
  cm.signal_last();
  EXPECT_EQ(4, cm.coroutines_created);
  EXPECT_EQ(4, cm.coroutines_suspended);
  EXPECT_EQ(2, cm.coroutines_signaled);
  EXPECT_EQ(2u, cm.waiting_size());
  EXPECT_EQ(4u, cm.total_coroutines());
  EXPECT_EQ(2u, cm.active_coroutines());
  EXPECT_EQ(0, cm.coroutines_finished);
  EXPECT_EQ(1, s.getInt("a"));
  EXPECT_EQ(2, s.getInt("b"));
  EXPECT_EQ(3, s.getInt("c"));
  EXPECT_EQ(4, s.getInt("d"));
  // Execute
  cm.update();
  EXPECT_EQ(4, cm.coroutines_created);
  EXPECT_EQ(4, cm.coroutines_suspended);
  EXPECT_EQ(2, cm.coroutines_signaled);
  EXPECT_EQ(2, cm.coroutines_finished);
  EXPECT_EQ(2u, cm.waiting_size());
  EXPECT_EQ(2u, cm.total_coroutines());
  EXPECT_EQ(0u, cm.active_coroutines());
  EXPECT_EQ(1, s.getInt("a"));
  EXPECT_EQ(2, s.getInt("b"));
  EXPECT_EQ(6, s.getInt("c"));
  EXPECT_EQ(8, s.getInt("d"));
  // END aborting two coroutines
  EXPECT_EQ(0, cm.coroutines_aborted);
  cm.finish();
  EXPECT_EQ(4, cm.coroutines_created);
  EXPECT_EQ(4, cm.coroutines_suspended);
  EXPECT_EQ(2, cm.coroutines_signaled);
  EXPECT_EQ(2, cm.coroutines_finished);
  EXPECT_EQ(2, cm.coroutines_aborted);
  EXPECT_EQ(2u, cm.waiting_size());
  EXPECT_EQ(0u, cm.total_coroutines());
  EXPECT_EQ(0u, cm.active_coroutines());
  EXPECT_EQ(1, s.getInt("a"));
  EXPECT_EQ(2, s.getInt("b"));
  EXPECT_EQ(6, s.getInt("c"));
  EXPECT_EQ(8, s.getInt("d"));

  // END
  cm.finish();
  EXPECT_EQ(false, cm.premature_lua_close);
  EXPECT_EQ(0, lua_gettop(s));
  s.finish();
}

TEST(Coroutines, waitfor_signal_abort_1) {
  Script s;
  SimpleSignalCM cm;

  s.init();
  cm.init(s);
  EXPECT_TRUE(Execute(s, "a,b = 0,0,0,0"));
  EXPECT_TRUE(Execute(s, "function test(v, add) _G[v]=_G[v]+add waitFor() _G[v]=_G[v]+add end"));
  // Start 2 threads
  EXPECT_TRUE(Execute(s, "async(test, 'a', 1)"));
  EXPECT_TRUE(Execute(s, "async(test, 'b', 2)"));
  EXPECT_EQ(2, cm.coroutines_created);
  EXPECT_EQ(0, cm.coroutines_suspended);
  EXPECT_EQ(0u, cm.waiting_size());
  EXPECT_EQ(2u, cm.total_coroutines());
  EXPECT_EQ(2u, cm.active_coroutines());
  // Execute coroutines, and add two more
  cm.update();
  EXPECT_EQ(2, cm.coroutines_created);
  EXPECT_EQ(2, cm.coroutines_suspended);
  EXPECT_EQ(0, cm.coroutines_aborted);
  EXPECT_EQ(2u, cm.waiting_size());
  EXPECT_EQ(2u, cm.total_coroutines());
  EXPECT_EQ(0u, cm.active_coroutines());
  EXPECT_EQ(1, s.getInt("a"));
  EXPECT_EQ(2, s.getInt("b"));
  // abort the last coroutine, signal the first one
  cm.abort_last();
  cm.signal_last();
  EXPECT_EQ(1, cm.coroutines_aborted);
  EXPECT_EQ(0, cm.coroutines_finished);
  EXPECT_EQ(1u, cm.total_coroutines());
  EXPECT_EQ(1u, cm.active_coroutines());
  cm.update();
  EXPECT_EQ(2, s.getInt("a"));
  EXPECT_EQ(2, s.getInt("b"));
  cm.finish();
  EXPECT_EQ(1, cm.coroutines_aborted);
  EXPECT_EQ(1, cm.coroutines_finished);
  EXPECT_EQ(0u, cm.total_coroutines());
  EXPECT_EQ(0u, cm.active_coroutines());
}

TEST(Coroutines, async_inside_async) {
  Script s;
  SimpleYieldCM cm;
  s.init();
  cm.init(s);
  unsigned level=5;
  EXPECT_TRUE(Execute(s, "count = 0"));
  EXPECT_TRUE(Execute(s, "function test(c) count = count +1 \n"
                         " for i=1,c do async(test, c-1) end end"));
  char buff[128];
  snprintf(buff, 128, "test(%d)", level);
  EXPECT_TRUE(Execute(s, buff));

  unsigned count = 0;
  unsigned max_bytes = 0;
  unsigned count_at_max = 0;
  for(unsigned i = level, total=level; i>0; i--, total=total*i) {
    count = count + total;
    EXPECT_EQ(total, cm.total_coroutines());
    EXPECT_EQ(total, cm.active_coroutines());
    cm.update();
    unsigned bytes = lua_gc(s, LUA_GCCOUNT, 0)*1024;
    bytes += lua_gc(s, LUA_GCCOUNTB, 0);
    if (bytes > max_bytes) {
      max_bytes = bytes;
      count_at_max = count;
    }
  }
  // Print some stats
  std::cout << "Stats:" << std::endl;
  std::cout << " - total coroutines created = " << count << std::endl;
  std::cout << " - max mem used = " << max_bytes << " bytes" << std::endl;
  std::cout << "    - coroutines at that moment =  " << count_at_max << std::endl;
  std::cout << "    - mem/coroutine = " << (float)max_bytes/count_at_max
                                        << " bytes/c" << std::endl;
  // Finish:
  cm.finish();
  EXPECT_EQ(0u, cm.total_coroutines());
  EXPECT_EQ(0u, cm.active_coroutines());
  EXPECT_EQ((int)count, cm.coroutines_created);
  EXPECT_EQ((int)count, cm.coroutines_finished);
  EXPECT_EQ((int)count+1, s.getInt("count"));
  EXPECT_EQ(false, cm.premature_lua_close);
  EXPECT_EQ(0, lua_gettop(s));
  s.finish();
}

TEST(Coroutines, test_coroutine_clear) {
  Script s;
  SimpleYieldCM cm;
  s.init();
  cm.init(s);
  //----------------------------------
  SLB3::Register<T1>(s);
  EXPECT_TRUE(Execute(s,"a=testing.T1()"));
  lua_getglobal(s, "a");
  T1 *a = SLB3::Get<T1*>(s, -1);
  lua_pop(s,1);
  //---------------------------------
  EXPECT_EQ(1, T1::Instance_count());
  EXPECT_TRUE(Execute(s, "async(function() local b=a waitFor() b:set_a(5) waitFor() b:set_a(2) end)"));
  //--1st func start copy on local upvalue a
  cm.update();
  EXPECT_TRUE(Execute(s,"a=nil"));
  lua_gc(s, LUA_GCCOLLECT, 0);
  EXPECT_EQ(1, T1::Instance_count());
  EXPECT_EQ(1, a->get_a());
  //--2nd func resumes from 1st waitfor
  cm.update();
  lua_gc(s, LUA_GCCOLLECT, 0);
  EXPECT_EQ(5, a->get_a());
  EXPECT_EQ(1, T1::Instance_count());
  //--3rd func resumes from 2nd waitfor and finishes
  cm.update();
  lua_gc(s, LUA_GCCOLLECT, 0);
  EXPECT_EQ(0, T1::Instance_count());
  //----------------------------------
  EXPECT_EQ(0, lua_gettop(s));
  cm.finish();
  s.finish();
  EXPECT_EQ(1, cm.coroutines_created);
  EXPECT_EQ(0, cm.coroutines_suspended);
  EXPECT_EQ(0, cm.coroutines_signaled);
  EXPECT_EQ(0, cm.coroutines_aborted);
  EXPECT_EQ(0, cm.coroutines_error);
  EXPECT_EQ(1, cm.coroutines_finished);
}

CoroutineManagerTestBase::CoroutineManagerTestBase() 
    : coroutines_created(0)
    , coroutines_suspended(0)
    , coroutines_signaled(0)
    , coroutines_finished(0)
    , coroutines_aborted(0)
    , coroutines_error(0)
    , premature_lua_close(false) {
  set_event_mask(kEvent_All);
}

void CoroutineManagerTestBase::event_implementation(
    const CoroutineManager::Event e,
    const Thread t,
    const char *error) {
  (void)t;
  (void)error;
  switch(e) {
    case kEvent_CoroutineCreated:
      coroutines_created++;
      break;
    case kEvent_CoroutineSuspended:
      coroutines_suspended++;
      break;
    case kEvent_CoroutineSignaled:
      coroutines_signaled++;
      break;
    case kEvent_CoroutineFinishes:
      coroutines_finished++;
      break;
    case kEvent_CoroutineAbort:
      coroutines_aborted++;
      break;
    case kEvent_CoroutineError:
      std::cout << "Error in coroutine: " << error << std::endl;
      coroutines_error++;
      break;
    case kEvent_PrematureCloseOfLuaState:
      premature_lua_close = true;
      break;
  }
}
