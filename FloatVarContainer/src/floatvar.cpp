#include "../include/floatvar.h"

namespace FVC {
  FloatVar::FloatVar() noexcept {
    mDataType.raw = gDefaultDataType;
  }
  
  bool FloatVar::isArray() noexcept const {
    return mBinaryLength >> MAX_BITS - 1;
  }
  
  length_t FloatVar::getSize() noexcept const {
    return (mBinaryLength << 1) >> 1;
  }
  
  FloatVar::~FloatVar() noexcept {
    if(maName) delete[] maName;
    
    if(isArray()) delete[] mData.aList;
    else          delete[] mData.aBinary;
    
  }

}
