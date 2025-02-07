#include "../include/floatvar.h"

namespace FVC {
  FloatVar::FloatVar(const char* string  , const char* name     , DataType data_type) : FloatVarHeader(name, data_type, FormatType::string) {
    mLength.length = strlen(string);
    copystring((char*)string);
  }
  
  FloatVar::FloatVar(const FloatVar* list, halfuint list_count  , const char* name  , DataType data_type) : FloatVarHeader(name, data_type, FormatType::list) {
    mData.aList = new FloatVar[mLength.capacity];
    memcpy(mData.aList, list, mLength.length * sizeof(FloatVar));
  }
  
  void FloatVar::clear() {
    if (mData.number) {
      if (isList()) delete[] mData.aList;
      else if (isArray()) delete[] (char*)mData.aArray;
      else if (isString()) delete[] mData.aString;
    }
    mData.number = 0;

    mLength.capacity = 0;
    mLength.length = 0;
  }

  void FloatVar::reserve(halfuint new_capacity) {
    if(mLength.length < new_capacity) reallocate(new_capacity);
  }

  void FloatVar::resize(halfuint new_length) {
    reallocate(new_length);
    mLength.length = new_length;
  }

  void FloatVar::shrinkToFit() {
    reallocate(mLength.length);
  }

  void FloatVar::push(const FloatVar& that) {
    if (!isList()) return;
    listexpansioncheck();

    mData.aList[mLength.length] = that;

    mLength.length++;
  }

  void FloatVar::push(char that) {
    if (!isString()) return;
    if (mLength.capacity < 2) reallocate(2);
    if (mLength.capacity - 1 == mLength.length) reallocate((mLength.capacity - 1) * 2);

    mData.aString[mLength.length] = that;

    mLength.length++;
  }

  void FloatVar::emplace(FloatVar&& that) {
    if (!isList()) return;
    listexpansioncheck();

    mData.aList[mLength.length] = std::move(that);

    mLength.length++;
  }

  inline FloatVar& FloatVar::operator[](halfuint index) {
    return ARAY_ACCESS(mData.aList, index, mLength.length);
  }

  const FloatVar& FloatVar::at(halfuint index) const {
    return ARAY_ACCESS(mData.aList, index, mLength.length);
  }

  inline char& FloatVar::strIndex(halfuint index) {
    return ARAY_ACCESS(mData.aString, index, mLength.length);
  }

  inline const char FloatVar::strIndexAt(halfuint index) const {
    return ARAY_ACCESS(mData.aString, index, mLength.length);
  }

  void FloatVar::operator=(const char* str) {
    restring((char*)str, strlen(str));
  }

  void FloatVar::restring(char* str, halfuint len) {
    if (!isString()) return;
    if (mData.aString) delete[] mData.aString;

    mLength.length = len;
    copystring(str);
  }

  bool FloatVar::operator==(const FloatVar& other) const {
    if (other.getFormat() != getFormat()) return false;

    bool same = true;
    if (isList()) {
      if (other.getSize() != mLength.length) return false;
      for (halfuint i = 0; i < mLength.length; i++) {
        same &= mData.aList[i] == other.mData.aList[i];
        if (!same) return false;
      }

      return true;
    }
    else if (isString()) {
      if (other.getSize() != mLength.length) return false;

      return strcmp(other.getString(), mData.aString) == 0;
    }
    else if (isNumber()) return other.mData.number == mData.number;
      
    return true;
  }

  FloatVar::~FloatVar() {
    if (mData.number && ((char)getFormat() >> 1)) {
      if (isList()) delete[] mData.aList;
      else if (isArray()) delete[] mData.aString;
      else if (isString()) delete[] mData.aString;
    }
  }

  inline void FloatVar::copystring(char* str) {
    mLength.capacity = mLength.length + 1;
    mData.aString = new char[mLength.capacity];
    mData.aString[mLength.length] = 0;
    memcpy(mData.aString, str, mLength.length);
  }

  void FloatVar::copyother(const FloatVar& other) {
    assignname(other.getName());
    mLength = other.mLength;
    retype(other.getType());

    switch (getFormat()) {
    case FormatType::list:
      mData.aList = new FloatVar[mLength.capacity];
      for (length_t i = 0; i < mLength.length; i++)
#pragma warning(suppress : 6385)
        mData.aList[i] = other.mData.aList[i];
      break;
    case FormatType::array:
      mData.aString = new char[mLength.length * mLength.offset];
      memcpy(mData.aString, other.getRawData(), mLength.length * mLength.offset);
      break;
    case FormatType::string:
      mData.aString = new char[mLength.capacity];
      memcpy(mData.aString, other.getString(), mLength.length);
      mData.aString[mLength.length] = 0;
      break;
    case FormatType::number:
      mData.number = other.mData.number;
      break;
    }    
  }

  void FloatVar::moveother(FloatVar&& other) {
    mLength      = other.mLength;
    mData.number = other.mData.number;

    other.mData.number = 0;

    moveotherheader(std::move(other));
  }

  inline void FloatVar::listexpansioncheck() {
    if (mLength.capacity < 2) reallocate(2);
    if (mLength.capacity == mLength.length) reallocate(mLength.capacity * 2);
  }

  void FloatVar::reallocate(halfuint new_capacity) {
    // we copy the minimal size so we can crop it out if it wouldn't fit
    void* new_buf;
    switch (getFormat()) {
    case FormatType::list:
      new_buf = new FloatVar[new_capacity];

      memcpy(new_buf, mData.aList, std::min(new_capacity, mLength.capacity) * sizeof(FloatVar));
      delete[] mData.aList;
      break;
    case FormatType::array:
      new_buf = new char[new_capacity * mLength.offset];

      memcpy(new_buf, mData.aArray, std::min(new_capacity, mLength.capacity) * mLength.offset);
      delete[] mData.aString;
      break;
    case FormatType::string:
      new_buf = new char[new_capacity + 1];
      ((char*)new_buf)[new_capacity] = 0;

      memcpy(new_buf, mData.aString, std::min(new_capacity, mLength.capacity));
      delete[] mData.aString;
      break;
    default:
      // wtf?
      abort();
      break;
    }
    mData.aString = (char*)new_buf;
    mLength.length = std::min(new_capacity, mLength.length);
    mLength.capacity = new_capacity;
  }
}
