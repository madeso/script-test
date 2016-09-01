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
#ifndef __TEST_T4__
#define __TEST_T4__ 1

#include "common.h"
#include "ref_ptr.h"

class T4Base : public virtual Referenced {
public:
  int get_a() const { return a_; }
  void set_a(int n) { a_ = n; }

  virtual const char *who() const = 0;

protected:
  T4Base() : a_(1) {}
  virtual ~T4Base() {}

private:
  int a_;

  T4Base(const T4Base&);
  T4Base& operator=(const T4Base&);
};

class T4 : public virtual T4Base, public ClassCounter<T4> {
public:
  T4() : b_(2) {}

  int get_b() const { return b_; }
  void set_b(int n) { b_ = n; }
  const char *who() const { return "T4"; }

protected:
  virtual ~T4() {}

private:
  int b_;
  float load_b_[50];

  T4(const T4&);
  T4& operator=(const T4&);
};

class T4_bis :public virtual T4 {
public:
  T4_bis() : c_(2) {}

  int get_c() const { return c_; }
  void set_c(int n) { c_ = n; }
  const char *who() const { return "T4_bis"; }

protected:
  virtual ~T4_bis() {}

private:
  int c_;
  float load_c_[15];

  T4_bis(const T4_bis&);
  T4_bis& operator=(const T4_bis&);
};

SLB3_DECLARATION(T4Base, "T4Base", policy::SmartPtr<ref_ptr>)
SLB3_DECLARATION(T4, "T4", policy::SmartPtr<ref_ptr>)
SLB3_DECLARATION(T4_bis, "T4_bis", policy::SmartPtr<ref_ptr>)

#endif
