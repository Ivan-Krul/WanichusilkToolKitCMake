#pragma once
#include "define.h"

#include <initializer_list>
#include <cstring>
#include <new>
#include <utility>

namespace FVC {
  // it's _smallarray because it uses the most efficient usage of aligned bytes
  // if 64-bit = 16 bytes with 8 bytes alignment
  // if 32-bit = 8  bytes with 4 bytes alignment
  
  template<typename Type>
  class _smallarray {
  public:
    _smallarray(halfuint capacity = 0) noexcept;
    _smallarray(const std::initializer_list<Type>& list) noexcept;
  
    inline void clear() noexcept;
    inline void resize(halfuint new_size) noexcept;
    
    inline void shrinkToFit() noexcept;
    
    void operator= (const _smallarray& entry) noexcept;
    void operator= (const std::initializer_list<Type>& list) noexcept;
    inline Type& operator[] (const halfuint index);
    inline Type& at(const halfuint index) const;
  
    void push(const Type& element);
    template<class... Args>
    void emplace(Args&&... args);
  
    inline Type* data() const noexcept { return (Type*)mArr; }
    inline halfuint size() const noexcept { return mSize; }
    inline halfuint capacity() const noexcept { return mCapacity; }
  
    ~_smallarray() {
      clear();
    }
  
  private:
    inline void deallocate();
    void allocate(halfuint new_cap);
  
    inline void* makeAlloc(const halfuint new_cap);
  
    void* mArr = nullptr;
    
    halfuint mSize = 0;
    halfuint mCapacity = 0;
  };
  
  template<typename Type>
  _smallarray<Type>::_smallarray(halfuint capacity) noexcept {
    if(!capacity) return;
    
    capacity = AVOID_MAX(capacity);
    
    mCapacity = capacity;
    mArr = makeAlloc(mCapacity);
  }
  
  template<typename Type>
  _smallarray<Type>::_smallarray(const std::initializer_list<Type>& list) noexcept {
    mCapacity = list.size();
    mCapacity = AVOID_MAX(capacity);
    mSize = mCapacity;
    
    mArr = makeAlloc(mCapacity);
    
    memcpy(mArr, list.data(), mCapacity * sizeof(Type));
  }
  
  template<typename Type>
  void _smallarray<Type>::clear() noexcept {
    deallocate();
    
    mArr = nullptr;
    mCapacity = 0;
    mSize = 0;
  }
  
  template<typename Type>
  void _smallarray<Type>::resize(halfuint new_size) noexcept {
    allocate(new_size);
    mSize = new_size;
  }
  
  template<typename Type>
  void _smallarray<Type>::shrinkToFit() noexcept {
    allocate(mSize);
  }
  
  template<typename Type>
  void _smallarray<Type>::push(const Type& element) {
    if(mSize == mCapacity) {
      mCapacity <<= 1;
      allocate(mCapacity + (mCapacity % 8));
    }
    
    Type* type = ((Type*)mArr)[mSize];
    *type = element;
    
    mSize++;
  }
  
  template<class Type> template<class ... Args>
  void _smallarray<Type>::emplace(Args&&... args) {
    if(mSize == mCapacity) {
      mCapacity <<= 1;
      allocate(mCapacity + (mCapacity % 8));
    }
    
    Type* type = ((Type*)mArr)[mSize];
    new (&type) Type(std::forward<Args>(args)...);
    
    mSize++;
  }
  
  template<typename Type>
  void _smallarray<Type>::operator= (const _smallarray& entry) noexcept {    
    deallocate();
    
    mSize = entry.size();
    mCapacity = mSize;
    
    mArr = makeAlloc(mCapacity);
    
    memcpy(mArr, entry.data(), mSize * sizeof(Type));
  }
  
  template<typename Type>
  void _smallarray<Type>::operator= (const std::initializer_list<Type>& list) noexcept {
    mCapacity = list.size();
    mSize = list.size();
    
    mArr = makeAlloc(mCapacity);
    
    memcpy(mArr, list.data(), mCapacity * sizeof(Type));
  }
  
  template<typename Type>
  Type& _smallarray<Type>::operator[] (const halfuint index) {
    if(index < mSize)
      return ((Type*)mArr)[index];

    return ((Type*)mArr)[0];
  }
  
  template<typename Type>
  Type& _smallarray<Type>::at(const halfuint index) const {
    if(index < mSize)
      return ((Type*)mArr)[index];

    return ((Type*)mArr)[0];
  }
  
  template<typename Type>
  void _smallarray<Type>::deallocate() {
    if(!mArr) return;
    
    if(~mSize == 0) mSize--;
    
    for(halfuint i = 0; i < mSize; i++) {
      (((Type*)mArr)[i]).~Type();
    }
    
    operator delete[](mArr);
  }
  
  template<typename Type>
  void _smallarray<Type>::allocate(halfuint new_cap) {
    if(new_cap == mCapacity) return;
    
    if(!new_cap) {
      clear();
      return;
    }
    
    new_cap = AVOID_MAX(new_cap);
    
    if(mCapacity > new_cap) mSize = new_cap;
    mCapacity = new_cap;
    
    Type* arr = (Type*)(makeAlloc(mCapacity));
    
    memcpy(arr, mArr, mSize * sizeof(Type));
    
    if(mArr) deallocate();
    
    mArr = arr;
  }
  
  template<typename Type>
  void* _smallarray<Type>::makeAlloc(const halfuint new_cap) {
#if __cplusplus < 201703L
    return operator new[](new_cap * sizeof(Type), alignof(Type));
#else
    return operator new[](new_cap * sizeof(Type));
#endif
  }
  
}
