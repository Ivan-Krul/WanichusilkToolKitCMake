#pragma once
#include "define.h"

#include <initializer_list>

namespace FVC {
  // it's _smallarray because it uses the most efficient usage of aligned bytes
  // if 64-bit = 16 bytes with 8 bytes alignment
  // if 32-bit = 8  bytes with 4 bytes alignment
  
  template<typename Type>
  class _smallarray {
  public:
    _smallarray(unsigned halfint capacity = 0) noexcept;
    _smallarray(const std::initializer_list<Type>& list) noexcept;
  
    void clear() noexcept;
    
    void operator= (const _smallarray& entry) noexcept;
    void operator= (const std::initializer_list<Type>& list) noexcept;
    Type& operator[] (const unsigned halfint index);
  
    Type* data() const noexcept;
    unsigned halfint size() const noexcept;
    unsigned halfint size() const noexcept;
  
  private:
    void allocate(halfint new_cap);
  
    Type* mArr = nullptr;
    
    unsigned halfint mSize = 0;
    unsigned halfint mCapacity = 0;
  };
  
  template<typename Type>
  _smallarray<Type>::_smallarray(unsigned halfint capacity) noexcept {
    if(!capacity) return;
    
    mCapacity = capacity;
    mArr = new Type[mCapacity];
  }
  
  template<typename Type>
  _smallarray<Type>::_smallarray(const std::initializer_list<Type>& list) noexcept {
    mCapacity = list.size();
    mSize = list.size();
    
    mArr = new Type[mCapacity];
    
    memcpy(mArr, list.data(), mCapacity * sizeof(Type));
  }
  
  template<typename Type>
  void _smallarray<Type>::clear() noexcept {
    delete[] mArr;
    mArr = nullptr;
    mCapacity = 0;
    mSize = 0;
  }
  
  template<typename Type>
  void _smallarray<Type>::operator= (const _smallarray& entry) noexcept {
    mSize = entry.size();
    mCapacity = mSize;
    delete[] mArr;
    mArr = new Type[mCapacity];
    memcpy(mArr, entry.data(), mSize * sizeof(Type));
  }
  
  void operator= (const std::initializer_list<Type>& list) noexcept {
    mCapacity = list.size();
    mSize = list.size();
    
    mArr = new Type[mCapacity];
    
    memcpy(mArr, list.data(), mCapacity * sizeof(Type));
  }
  
  Type& operator[] (const unsigned halfint index) {
    if(index < mSize)
      return mArr[index];

    return mArr[0];
  }
  
  template<typename Type>
  Type* _smallarray<Type>::data() const noexcept {
    return mArr;
  }
  
  template<typename Type>
  unsigned halfint _smallarray<Type>::size() const noexcept {
    return mSize;
  }
  
  template<typename Type>
  void _smallarray<Type>::allocate(unsigned halfint new_cap) {
    if(new_cap == mCapacity) return;
    
    if(!new_cap) {
      clear();
      return;
    }
    
    if(mArr) delete[] mArr;
    
    if(mCapacity > new_cap) mSize = new_cap;
    
    mCapacity = new_cap;
    mArr = new Type[mCapacity];
  }
  
}
