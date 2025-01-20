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
    copyother(other);
  }
  
  void FloatVar::clear() {
    if (mData.number) {
      if (isArray()) delete[] mData.aList;
      else if (isString()) delete[] mData.aString;
      else if (isNumber()) mData.number = 0;
    }

    mLength.capacity = 0;
    mLength.length = 0;
  }

  void FloatVar::reserve(halfuint new_capacity) {
    reallocate(new_capacity);
  }

  void FloatVar::resize(halfuint new_length) {
    reallocate(new_length);
    mLength.length = new_length;
  }

  void FloatVar::shrinkToFit() {
    reallocate(mLength.length);
  }

  void FloatVar::push(const FloatVar& that) {
    if (mLength.type != FormatType::array) return;
    if (mLength.capacity == mLength.length) reallocate(mLength.capacity * 2);

    mData.aList[mLength.length] = that;

    mLength.length++;
  }

  void FloatVar::push(char that) {
    if (mLength.type != FormatType::array) return;
    if (mLength.capacity - 1 == mLength.length) reallocate((mLength.capacity - 1) * 2);

    mData.aString[mLength.length] = that;

    mLength.length++;
  }

  void FloatVar::embrace(FloatVar&& that) {
    if (mLength.type != FormatType::array) return;
    if (mLength.capacity == mLength.length) reallocate(mLength.capacity * 2);

    mData.aList[mLength.length] = std::move(that);

    mLength.length++;
  }

  bool FloatVar::operator==(const FloatVar& other) const {
    if (other.getFormat() != mLength.type) return false;

    bool same = true;
    if (isArray()) {
      if (other.getSize() != mLength.length) return false;
      for (halfuint i = 0; i < mLength.length; i++)
        same &= mData.aList[i] == other.mData.aList[i];

      return same;
    }
    else if (isString()) {
      if (other.getSize() != mLength.length) return false;

      return strcmp(other.getString(), mData.aString) == 0;
    }
    else if (isNumber()) return other.mData.number == mData.number;
      
    return true;
  }

  FloatVar::~FloatVar() {
    if(maName)    delete[] maName;
    
    if (mData.number) {
      if (isArray()) delete[] mData.aList;
      else if (isString()) delete[] mData.aString;
    }

  }
  
  void FloatVar::assignname(const char* name) {
    if (name) delete[] name;

    mLength.length = strlen(name);
    maName = new char[mLength.length + 1];
    memcpy(maName, name, mLength.length);
    maName[mLength.length] = 0;
  }

  void FloatVar::copyother(const FloatVar& other) {
    assignname(other.getName());
    mLength = other.mLength;
    mDataType = other.getType();

    if (other.isArray()) {
      mData.aList = new FloatVar[mLength.capacity];
      memcpy(mData.aList, other.mData.aList, mLength.length);
    } else if (other.isString()) {
      mData.aString = new char[mLength.capacity];
      memcpy(mData.aString, other.getString(), mLength.length);
      mData.aString[mLength.length] = 0;
    } else if (other.isNumber())
      mData.number = mData.number;
  }

  void FloatVar::moveother(FloatVar&& other) {
    mLength = other.mLength;
    mData.number = other.mData.number;
    mDataType.dt.raw = other.mDataType.dt.raw;
    maName = other.maName;

    other.mData.number = 0;
    other.maName = nullptr;
  }

  void FloatVar::reallocate(halfuint new_capacity) {
    // we copy the minimal size so we can crop it out if it wouldn't fit
    void* new_buf;
    switch (mLength.type) {
    case FormatType::array:
      new_buf = new FloatVar[new_capacity];

      memcpy(new_buf, mData.aList, std::min(new_capacity, mLength.capacity) * sizeof(FloatVar));

      delete[] mData.aList;

      mData.aList = (FloatVar*)new_buf;
      break;
    case FormatType::string:
      new_buf = new char[new_capacity +1];
      ((char*)new_buf)[new_capacity] = 0;

      memcpy(new_buf, mData.aString, std::min(new_capacity, mLength.capacity));

      delete[] mData.aString;

      mData.aString = (char*)new_buf;
      break;
    }
    mLength.length = std::min(new_capacity, mLength.length);
    mLength.capacity = new_capacity;
  }
}
