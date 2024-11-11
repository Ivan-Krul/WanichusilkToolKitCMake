#pragma once
#include "define.h"

#include <initializer_list>
#include <cstring>
#include <new>

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
    inline Type& operator[] (const halfuint index) noexcept;
    inline Type& at(const halfuint index) const noexcept;
  
    void push(const Type& element);
    template<class... Args>
    void emplace(Args&&... args);
  
    inline Type* data() const noexcept { return (Type*)mArr; }
    inline halfuint size() const noexcept { return mSize; }
    inline halfuint capacity() const noexcept { return mCapacity; }
  
    ~smallarray() {
    }
  
  private:
    void deallocate();
    void allocate(const halfuint new_cap);
  
    void* mArr = nullptr;
    
    halfuint mSize = 0;
    halfuint mCapacity = 0;
  };
  
  template<typename Type>
  _smallarray<Type>::_smallarray(halfuint capacity) noexcept {
    if(!capacity) return;
    
    capacity = AVOID_MAX(capacity);
    
    mCapacity = capacity;
    mArr = operator new[](mCapacity * sizeof(Type), alignof(Type));
  }
  
  template<typename Type>
  _smallarray<Type>::_smallarray(const std::initializer_list<Type>& list) noexcept {
    mCapacity = list.size();
    mCapacity = AVOID_MAX(capacity);
    mSize = mCapacity;
    
    mArr = operator new[](mCapacity * sizeof(Type), alignof(Type));
    
    memcpy(mArr, list.data(), mCapacity * sizeof(Type));
  }
  
  template<typename Type>
  void _smallarray<Type>::clear() noexcept {
    operator delete[](mArr);
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
    
    Type* type = mArr[mSize];
    *type = element;
    
    mSize++;
  }
  
  template<typename Type, class... Args>
  void _smallarray<Type>::emplace(Args&&... args) {
    if(mSize == mCapacity) {
      mCapacity <<= 1;
      allocate(mCapacity + (mCapacity % 8));
    }
    
    Type* type = mArr[mSize];
    new (&type) Type(std::forward<TArgs>(m_args)...);
    
    mSize++;
  }
  
  template<typename Type>
  void _smallarray<Type>::operator= (const _smallarray& entry) noexcept {
    mSize = entry.size();
    mCapacity = mSize;
    operator delete[](mArr);
    mArr = operator new[](mCapacity * sizeof(Type), alignof(Type));
    memcpy(mArr, entry.data(), mSize * sizeof(Type));
  }
  
  template<typename Type>
  void _smallarray<Type>::operator= (const std::initializer_list<Type>& list) noexcept {
    mCapacity = list.size();
    mSize = list.size();
    
    mArr = operator new[](mCapacity * sizeof(Type), alignof(Type));
    
    memcpy(mArr, list.data(), mCapacity * sizeof(Type));
  }
  
  template<typename Type>
  Type& _smallarray<Type>::operator[] (const halfuint index) noexcept {
    if(index < mSize)
      return ((Type*)mArr)[index];

    return ((Type*)mArr)[0];
  }
  
  template<typename Type>
  inline Type& at(const halfuint index) const noexcept {
    if(index < mSize)
      return ((Type*)mArr)[index];

    return ((Type*)mArr)[0];
  }
  
  template<typename Type>
  void _smallarray<Type>::deallocate() {
    if(!mArr) return;
    
    if(~mSize == 0) mSize--;
    
    for(halfuint i = 0; i < mSize; ) {
      
    }
    
    operator delete[](mArr);
  }
  
  template<typename Type>
  void _smallarray<Type>::allocate(const halfuint new_cap) {
    if(new_cap == mCapacity) return;
    
    if(!new_cap) {
      clear();
      return;
    }
    
    new_cap = AVOID_MAX(new_cap);
    
    if(mCapacity > new_cap) mSize = new_cap;
    mCapacity = new_cap;
    
    Type* arr = operator new[](mCapacity, alignof(Type));
    
    memcpy(arr, mArr, mSize * sizeof(Type));
    
    if(mArr) operator delete[](mArr);
    
    mArr = arr;
  }
  
}
