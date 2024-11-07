#pragma once
#include "define.h"

#include <initializer_list>
#include <cstring>

namespace FVC {
  // it's _smallarray because it uses the most efficient usage of aligned bytes
  // if 64-bit = 16 bytes with 8 bytes alignment
  // if 32-bit = 8  bytes with 4 bytes alignment
  
  template<typename Type>
  class _smallarray {
  public:
    _smallarray(halfuint capacity = 0) noexcept;
    _smallarray(const std::initializer_list<Type>& list) noexcept;
  
    void clear() noexcept;
    void resize(halfuint new_size) noexcept;
    
    void operator= (const _smallarray& entry) noexcept;
    void operator= (const std::initializer_list<Type>& list) noexcept;
    Type& operator[] (const halfuint index);
  
    inline Type* data() const noexcept { return mArr; }
    inline halfuint size() const noexcept { return mSize; }
    inline halfuint capacity() const noexcept { return mCapacity; }
  
  private:
    void allocate(const halfint new_cap);
  
    Type* mArr = nullptr;
    
    halfuint mSize = 0;
    halfuint mCapacity = 0;
  };
  
  template<typename Type>
  _smallarray<Type>::_smallarray(halfuint capacity) noexcept {
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
  void _smallarray<Type>::resize(halfuint new_size) noexcept {
    allocate(new_size);
  }
  
  template<typename Type>
  void _smallarray<Type>::operator= (const _smallarray& entry) noexcept {
    mSize = entry.size();
    mCapacity = mSize;
    delete[] mArr;
    mArr = new Type[mCapacity];
    memcpy(mArr, entry.data(), mSize * sizeof(Type));
  }
  
  template<typename Type>
  void _smallarray<Type>::operator= (const std::initializer_list<Type>& list) noexcept {
    mCapacity = list.size();
    mSize = list.size();
    
    mArr = new Type[mCapacity];
    
    memcpy(mArr, list.data(), mCapacity * sizeof(Type));
  }
  
  template<typename Type>
  Type& _smallarray<Type>::operator[] (const halfuint index) {
    if(index < mSize)
      return mArr[index];

    return mArr[0];
  }
  
  template<typename Type>
  void _smallarray<Type>::allocate(const halfint new_cap) {
    if(new_cap == mCapacity) return;
    
    if(!new_cap) {
      clear();
      return;
    }
    
    if(mCapacity > new_cap) mSize = new_cap;
    mCapacity = new_cap;
    
    Type* arr = new Type[mCapacity];
    
    memcpy(arr, mArr, mSize * sizeof(Type));
    
    if(mArr) delete[] mArr;
    
    mArr = arr;
  }
  
}
