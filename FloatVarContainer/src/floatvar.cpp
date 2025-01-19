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
  
  void FloatVar::clear() {
    if (isArray()) delete[] mData.aList;
    else if (isString()) delete[] mData.aString;
    else if (isNumber()) mData.number = 0;

    mLength.capacity = 0;
    mLength.length = 0;
  }

  void FloatVar::resize(halfuint new_length) {
    mLength.length = new_length;
    if (isArray()) {
      auto new_arr = new FloatVar[new_length];
      memcpy(new_arr, mData.aList, std::min(new_length, mLength.capacity));

      delete[] mData.aList;

      mData.aList = new_arr;
      mLength.capacity = new_length;
    }
    else if (isString()) {
      auto new_str = new char[new_length+1];
      new_str[new_length] = 0;
      memcpy(new_str, mData.aString, std::min(new_length, mLength.capacity));

      delete[] mData.aString;

      mData.aString = new_str;
      mLength.capacity = new_length + 1;
    }
  }

  void FloatVar::shrinkToFit() {
    if (isArray()) {
      auto new_arr = new FloatVar[mLength.length];
      memcpy(new_arr, mData.aList, mLength.length);

      delete[] mData.aList;

      mData.aList = new_arr;
    } else if (isString()) {
      auto new_str = new char[mLength.length + 1];
      new_str[mLength.length] = 0;
      memcpy(new_str, mData.aString, mLength.length);

      delete[] mData.aString;

      mData.aString = new_str;
    }
  }

  FloatVar::~FloatVar() {
    if(maName)    delete[] maName;
    
    if(isArray()) delete[] mData.aList;
    else if(isString()) delete[] mData.aString;
    
  }
  
  void FloatVar::assignname(const char* name) {
    if (name) delete[] name;

    mLength.length = strlen(name);
    maName = new char[mLength.length + 1];
    memcpy(maName, name, mLength.length);
    maName[mLength.length] = 0;
  }

  void FloatVar::reallocate(halfuint new_length) {
    switch (mLength.type) {
    case FormatType::array:
      // continue here
      break;
    }
  }

}
