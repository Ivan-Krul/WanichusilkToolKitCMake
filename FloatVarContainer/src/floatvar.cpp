#include "../include/floatvar.h"

namespace FVC {
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
  
  FloatVar::~FloatVar() {
    if(maName)    delete[] maName;
    
    if(isArray()) delete[] mData.aList;
    else if(isString()) delete[] mData.aString;
    
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
