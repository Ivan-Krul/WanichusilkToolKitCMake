#include "../include/floatvar.h"

namespace FVC {
  FloatVar::FloatVar() noexcept {
    mDataType.raw = gDefaultDataType;
  }
  
  FloatVar::FloatVar(const char* name     , DataType data_type) {
    assignname(name);
    mLength.type = FormatType::none;
    mDataType = data_type;
  }
  
  FloatVar::FloatVar(const char* string  , const char* name     , DataType data_type) {
    assignname(name);
    mLength.type = FormatType::string;
    
    mLength.length = strlen(binary);
    mLength.capacity = mLength.length + 1;
    ltoc(mLength.capacity);
    
    mData.aString = new char[mLength.capacity];
    memcpy(mData.aString, binary, mLength.length);    
    mData.aString[mLength.length] = 0;
    
    mDataType = data_type;
  }
  
  template<typename T>
  typename std::enable_if_t<std::is_arithmetic_v<T>>
  FloatVar::FloatVar(T number  , halfuint binary_count, const char* name  , DataType data_type) {
    assignname(name);
    mLength.type = FormatType::number;
    
    mLength.length = sizeof(T);
    
    mData.number = 0;
    memcpy(mData.number, number, sizeof(T));
    
    mDataType = data_type;
  }
  
  FloatVar::FloatVar(const FloatVar* list, halfuint list_count  , const char* name  , DataType data_type) noexcept {
    assignname(name);
    mLength.type = FormatType::array;
    
    mLength.length = list_count;
    mLength.capacity = mLength.length;
    ltoc(mLength.capacity);
    
    mData.aList = new FloatVar[mLength.capacity];
    memcpy(mData.aList, list, mLength.length);
    
    mDataType = data_type;
  }
  
  FormatType getFormat() noexcept const {
    return mFormatType;
  }
  
  bool FloatVar::isArray() noexcept const {
    return mLength.type == FormatType::array;
  }
  
  bool FloatVar::isString() noexcept const {
    return mLength.type == FormatType::string;
  }
  
  bool FloatVar::isNumber() noexcept const {
    return mLength.type == FormatType::number;
  }
  
  length_t FloatVar::getSize() noexcept const {
    return mLength.length;
  }
  
  FloatVar::~FloatVar() noexcept {
    if(maName)    delete[] maName;
    
    if(isArray()) delete[] mData.aList;
    else          delete[] mData.aString;
    
  }
  
  void FloatVar::ctol(halfuint& cap) {
    cap <<= 2;
  }
  
  void FloatVar::ltoc(halfuint& len) {
    len = (len >> 2) + 1;
  }
  
  void FloatVar::assignname(const char* name) {
    mLength.length = strlen(name);
    maName = new char[mLength.length + 1];
    memcpy(maName, name, mLength.length)
    maName[mLength.length] = 0;
  }

}
