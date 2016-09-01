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
#include "gtest/gtest.h"
#include <SLB3/namespace.h>
#include <SLB3/extra/coroutinemanager.h>

using namespace SLB3::extra;

// Description of a Channel table ----------------
// Channel {
//   send_queue = {thread = data, thread = data, ...}
//   recv_queue = {thread = true, thread = true, ...}
//   }

class SendReceiveCM: public CoroutineManager {
public:
  SendReceiveCM() {}
  ~SendReceiveCM() {}
protected:
  enum WaitForTypes {
    kWait_Send,
    kWait_Recv,
  };

  void init_implementation();
  WaitForResult waitFor_implementation(Thread t, int id);

};

static int Lua_CreateChannel(lua_State*);

void SendReceiveCM::init_implementation() {
  lua_State* L = lua_state();
  pushLua_async(L);
  lua_setglobal(L, "async");

  lua_newtable(L); //< will be global channel table
  luaL_newmetatable(L, "channel");
  // __index 
  lua_newtable(L);
  pushLua_waitFor(L, kWait_Recv);
  lua_setfield(L, -2, "recv");
  pushLua_waitFor(L, kWait_Send);
  lua_setfield(L, -2, "send");
  lua_setfield(L,-2, "__index");
  // __call
  lua_pushlightuserdata(L, this);
  lua_pushcclosure(L, Lua_CreateChannel, 1);
  lua_setfield(L, -2, "__call");
  // end of channel metatable
  lua_setmetatable(L,-2);
  lua_setglobal(L, "channel");
}

int Lua_CreateChannel(lua_State *L) {
  lua_newtable(L);
  lua_newtable(L);
  lua_setfield(L,-2, "send_queue");
  lua_newtable(L);
  lua_setfield(L, -2, "recv_queue");
  luaL_getmetatable(L, "channel");
  lua_setmetatable(L, -2);
  return 1;
}

CoroutineManager::Thread ChannelPushNextSendData(lua_State *L, int channel_idx) {
  int top = lua_gettop(L);
  lua_getfield(L, channel_idx, "send_queue");
  lua_pushnil(L);
  if (lua_next(L,-2)) {
    //key = sender_thread
    //value = data sent
    CoroutineManager::Thread thread = (CoroutineManager::Thread) lua_topointer(L,-2);
    lua_insert(L, top+1);
    // remove the element from the table
    lua_pushnil(L);
    lua_rawset(L,-3);
    // return
    lua_settop(L, top+1);
    return thread;
  } else {
    lua_settop(L,top);
    return NULL;
  }
}

CoroutineManager::Thread ChannelGetNextRecv(lua_State *L, int channel_idx) {
  CoroutineManager::Thread result = NULL;
  int top = lua_gettop(L);
  lua_getfield(L, channel_idx, "recv_queue");
  lua_pushnil(L);
  if (lua_next(L,-2)) {
    result = (CoroutineManager::Thread) lua_topointer(L,-2);
    lua_pop(L,1); // value
    lua_pushnil(L);
    lua_rawset(L,-3); // remove the element from the table
  }
  lua_settop(L, top);
  return result;
}

CoroutineManager::WaitForResult SendReceiveCM::waitFor_implementation(
    CoroutineManager::Thread L,
    int action_id) {
  // Check if the first argument is a "channel" -----------------------
  luaL_getmetatable(L, "channel");
  lua_getmetatable(L, 1);
  int is_channel = lua_rawequal(L,-1,-2);
  lua_pop(L,2);
  // ------------------------------------------------------------------
  if (is_channel) {
    Thread peer = NULL;
    switch(action_id) {
      case kWait_Recv:
        // 1st check if there is any send available
        peer = ChannelPushNextSendData(L,1);
        if (peer) {
          signalCoroutine(peer);
          assert(lua_gettop(peer) == 0 && "Sender thread-top should be 0");
          // place "data" from sender as first argument to return
          lua_replace(L,1);
          lua_settop(L,1);
          return CoroutineManager::kW_ContinueExecution;
        } else {
          lua_getfield(L,1, "recv_queue");
          lua_pushlightuserdata(L, L);
          lua_pushboolean(L,1);
          lua_rawset(L,-3);
          lua_settop(L,0); // wipe the stack
          return CoroutineManager::kW_WaitForSignal;
        }
        break;
      case kWait_Send:
        // Trim the stack to SENDER, ARGUMENT
        lua_settop(L,2); 
        peer = ChannelGetNextRecv(L,1);
        if (peer) {
          assert(lua_gettop(peer) == 0 && "Receive thread-top should be 0");
          // move the argument from one thread to the other
          lua_xmove(L, peer, 1);
          // signal our peer so it can continue execution
          signalCoroutine(peer);
          // trim our stack and continue
          lua_settop(L,0);
          return CoroutineManager::kW_ContinueExecution;
        } else {
          lua_getfield(L,1, "send_queue");
          lua_pushlightuserdata(L, L);
          lua_pushvalue(L,2);
          lua_rawset(L,-3);
          lua_settop(L,0); // wipe the stack
          return CoroutineManager::kW_WaitForSignal;
        }
        break;
    }
    assert(!"Should never get here");
  }
  // Error:
  lua_pushstring(L, "Missing channel as #1 arg");
  return CoroutineManager::kW_Error;
}


TEST(SendReceiveCM, basic) {
  Script s;
  SendReceiveCM cm;
  s.init();
  cm.init(s);
  EXPECT_TRUE(Execute(s, "a = channel()"));
  EXPECT_TRUE(Execute(s, "async(function(c) c:send(5) end, a)"));
  EXPECT_TRUE(Execute(s, "async(function(c) result = c:recv() end, a)"));

  unsigned loop_count = 0;
  while(loop_count < 100 && (cm.total_coroutines() > 0)) {
    cm.update();
    loop_count++;
  }

  EXPECT_EQ(2u, loop_count);
  EXPECT_EQ(5, s.getInt("result"));
}


TEST(SendReceiveCM, basic_2) {
  Script s;
  SendReceiveCM cm;
  s.init();
  cm.init(s);
  EXPECT_TRUE(Execute(s, "a = channel()"));
  EXPECT_TRUE(Execute(s, "async(function(c) result = c:recv()+c:recv() end, a)"));
  EXPECT_TRUE(Execute(s, "async(function(c) c:send(5) end, a)"));
  EXPECT_TRUE(Execute(s, "async(function(c) c:send(15) end, a)"));

  unsigned loop_count = 0;
  while(loop_count < 100 && (cm.total_coroutines() > 0)) {
    cm.update();
    loop_count++;
  }

  EXPECT_LE(loop_count, 3u);
  EXPECT_EQ(20, s.getInt("result"));
}

TEST(SendReceiveCM, basic_3) {
  Script s;
  SendReceiveCM cm;
  s.init();
  cm.init(s);
  EXPECT_TRUE(Execute(s, "a = channel()"));
  EXPECT_TRUE(Execute(s, "async(function(c) c:send(5) result = c:recv() end, a)"));
  EXPECT_TRUE(Execute(s, "async(function(c) local v = c:recv() c:send(v*2) end, a)"));

  unsigned loop_count = 0;
  while(loop_count < 100 && (cm.total_coroutines() > 0)) {
    cm.update();
    loop_count++;
  }

  EXPECT_EQ(loop_count, 3u);
  EXPECT_EQ(10, s.getInt("result"));
}
