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
// Copyright 2012 WeUsePowerups Ent.
// all rights reserved
// author: Jose L. Hidalgo <jlhidalgo@weusepowerups.com>
//
// ref_ptr from SLB (http://http://code.google.com/p/slb/)
//
#ifndef INCLUDE_POWERUP_CORE_REF_PTR_H_
#define INCLUDE_POWERUP_CORE_REF_PTR_H_

  class Referenced {
    public:
    void ref() { ref_count_++; }
    void unref() {
      ref_count_--;
      if (ref_count_ == 0) {
        delete this;
      }
    }
    unsigned count() const { return ref_count_; }
  protected:
    Referenced() : ref_count_(0) {}
    virtual ~Referenced() {}
  private:
    unsigned ref_count_;
  };

  template<class T>
  class ref_ptr {
  public:
    typedef T element_type;

    ref_ptr() :ptr_(0L) {}
    
    ref_ptr(const ref_ptr& rp):ptr_(rp.ptr_)  { if (ptr_) ptr_->ref(); }
    ~ref_ptr() {
      if (ptr_) ptr_->unref();
      ptr_ = 0L;
    }

    ref_ptr& operator = (const ref_ptr& rp) {
      if (ptr_ == rp.ptr_) return *this;
      T* tmp_ptr = ptr_;
      ptr_ = rp.ptr_;
      if (ptr_) ptr_->ref();
      if (tmp_ptr) tmp_ptr->unref();
      return *this;
    }

    T* alloc() {
      if (ptr_) ptr_->unref();
      ptr_ = new T();
      ptr_->ref();
      return ptr_;
    }

    ref_ptr(T* ptr) : ptr_(ptr) { if (ptr_) ptr_->ref(); }

    ref_ptr& operator = (T* ptr) {
      if (ptr_ == ptr) return *this;
      T* tmp_ptr = ptr_;
      ptr_ = ptr;
      if (ptr_) ptr_->ref();
      if (tmp_ptr) tmp_ptr->unref();
      return *this;
    }

    // comparison operators for ref_ptr.
    bool operator == (const ref_ptr& rp) const { return (ptr_ == rp.ptr_); }
    bool operator != (const ref_ptr& rp) const { return (ptr_ != rp.ptr_); }
    bool operator < (const ref_ptr& rp) const { return (ptr_ < rp.ptr_); }
    bool operator > (const ref_ptr& rp) const { return (ptr_ > rp.ptr_); }

    // comparison operator for const T*.
    bool operator == (const T* ptr) const { return (ptr_ == ptr); }
    bool operator != (const T* ptr) const { return (ptr_ != ptr); }
    bool operator < (const T* ptr) const { return (ptr_ < ptr); }
    bool operator > (const T* ptr) const { return (ptr_ > ptr); }

    T& operator*() { return *ptr_; }
    const T& operator*() const { return *ptr_; }
    T* operator->() { return ptr_; }
    const T* operator->() const { return ptr_; }
    bool operator!() const { return ptr_ == 0L; }
    bool valid() const { return ptr_ != 0L; }
    T* get() { return ptr_; }
    const T* get() const { return ptr_; }

  private:
    T* ptr_;
  };
#endif  // INCLUDE_POWERUP_CORE_REF_PTR_H_
