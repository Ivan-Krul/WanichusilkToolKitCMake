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
    mLength.capacity = mLength.length + 1;
    
    mData.aString = new char[mLength.capacity];
    memcpy(mData.aString, string, mLength.length);    
    mData.aString[mLength.length] = 0;
    
    mDataType = data_type;
  }
  
  FloatVar::FloatVar(const FloatVar* list, halfuint list_count  , const char* name  , DataType data_type) {
    assignname(name);
    mLength.type = FormatType::array;
    
    mLength.length = list_count;
    mLength.capacity = mLength.length;
    
    mData.aList = new FloatVar[mLength.capacity];
    memcpy(mData.aList, list, mLength.length);
    
    mDataType = data_type;
  }
  
  FloatVar::FloatVar(const FloatVar& other) {
    assignname(other.getName());
    mLength = other.mLength;
    mDataType = other.getType();

    if (other.isArray()) {
      mData.aList = new FloatVar[mLength.capacity];
      memcpy(mData.aList, other.mData.aList, mLength.length);
    }
    else if (other.isString()) {
      mData.aString = new char[mLength.capacity];
      memcpy(mData.aString, other.getString(), mLength.length);
      mData.aString[mLength.length] = 0;
    }
    else if (other.isNumber()) {
      memcpy(&mData.number, (void*)(&other.mData.number), 8);
    }
  }
  
  FloatVar::~FloatVar() {
    if(maName)    delete[] maName;
    
    if(isArray()) delete[] mData.aList;
    else if(isString()) delete[] mData.aString;
    
  }
  
  void FloatVar::assignname(const char* name) {
    mLength.length = strlen(name);
    maName = new char[mLength.length + 1];
    memcpy(maName, name, mLength.length);
    maName[mLength.length] = 0;
  }

}
