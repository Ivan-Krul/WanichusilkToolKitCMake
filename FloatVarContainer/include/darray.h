#pragma once
#include <define.h>

namespace FVC {
  template<typename Type>
  class _smallarray {
  public:
    _smallarray(unsigned halfint capacity = 0) noexcept;
  
    
  
  private:
    void allocate(halfint new_cap);
  
    Type* mArr = nullptr;
    
    unsigned halfint mSize = 0;
    unsigned halfint mCapacity = 0;
  };
  
  _smallarray::_smallarray(unsigned halfint capacity) {
    if(!capacity) return;
    
    mCapacity = capacity;
    mArr = new Type[mCapacity];
  }
  
  
  _smallarray::allocate(unsigned halfint new_cap) {
    if(new_cap == mCapacity) return;
    
    if(!new_cap) {
      delete[] mArr;
      mArr = nullptr;
      mCapacity = 0;
      mSize = 0;
      return;
    }
    
    if(mArr) delete[] mArr;
    
    if(mCapacity > new_cap) mSize = new_cap;
    
    mCapacity = new_cap;
    mArr = new Type[mCapacity];
  }
  
}
