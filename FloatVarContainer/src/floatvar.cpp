#include "../include/floatvar.h"

namespace FVC {
  FloatVar::FloatVar() {
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
    
    mLength.length = strlen(string);

    auto cap = mLength.length + 1;
    ltoc(cap);
    mLength.capacity = cap;
    
    mData.aString = new char[mLength.capacity];
    memcpy(mData.aString, string, mLength.length);    
    mData.aString[mLength.length] = 0;
    
    mDataType = data_type;
  }
  
  template<typename T>
  typename std::enable_if<std::is_arithmetic<T>::value>::type
  FloatVar::FloatVar(T number  , halfuint binary_count, const char* name  , DataType data_type) {
    assignname(name);
    mLength.type = FormatType::number;
    
    mLength.length = sizeof(T);
    
    mData.number = 0;
    memcpy(mData.number, number, sizeof(T));
    
    mDataType = data_type;
  }
  
  FloatVar::FloatVar(const FloatVar* list, halfuint list_count  , const char* name  , DataType data_type) {
    assignname(name);
    mLength.type = FormatType::array;
    
    mLength.length = list_count;
    
    auto cap = mLength.length;
    ltoc(cap);
    mLength.capacity = cap;
    
    mData.aList = new FloatVar[mLength.capacity];
    memcpy(mData.aList, list, mLength.length);
    
    mDataType = data_type;
  }
  
  FloatVar::FloatVar(const FloatVar& other) {
    assignname(other.getName());
    
  }
  
  FloatVar::FormatType FloatVar::getFormat() const {
    return LengthStruct.type;
  }
  
  bool FloatVar::isArray() const {
    return mLength.type == FormatType::array;
  }
  
  bool FloatVar::isString() const {
    return mLength.type == FormatType::string;
  }
  
  bool FloatVar::isNumber() const {
    return mLength.type == FormatType::number;
  }
  
  halfuint FloatVar::getSize() const {
    return mLength.length;
  }
  
  const char* FloatVar::getName() const {
    return maName;
  }
  
  template<typename T>
  typename std::enable_if<std::is_arithmetic<T>::value,T>::type FloatVar::getNumber() const {
    if(isNumber())
      return *((T*)&mData.number);
    else
      return 0;
  }
  
  FloatVar::~FloatVar() {
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
    memcpy(maName, name, mLength.length);
    maName[mLength.length] = 0;
  }

}
