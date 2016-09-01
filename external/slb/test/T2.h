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
#ifndef __TEST_T2__
#define __TEST_T2__ 1

#include "common.h"
#include "T1.h"

class T2 : public ClassCounter<T2> {
public:
  T2(const char *txt) : txt_(txt), t1_(NULL) { t1_ = new T1(); }
  ~T2() {
    if (t1_) { delete t1_; t1_ = NULL; }
  }

  T1* get_t1_ptr() { return t1_; }
  T1& get_t1_ref() { return *t1_; }
  const T1* get_t1_const_ptr() const { return t1_; }
  const T1& get_t1_const_ref() const { return *t1_; }
  T1 get_t1_copy() const { return *t1_; }

  void set_txt(const char *txt) { txt_ = txt; }
  const char *txt() const { return txt_; }

private:
  const char *txt_;
  T1* t1_;

  T2(const T2&);
  T2& operator=(const T2&);
};

SLB3_DECLARATION(T2, "T2", policy::Default)


#endif
