#pragma once
#include <stdint.h>
#include <string.h>
#include <memory>
#include <type_traits>

#include "define.h"

namespace FVC {
#define FLOAT_VAR_INDEX_GUARD

  constexpr int gDefaultDataType = 0x766F6964; // void
  
  class FloatVar {
  public:
    union uDataType {
      char sym[4];
      int raw;
    };

    struct DataType {
      uDataType dt;
      inline DataType() { dt.raw = gDefaultDataType; }
      inline DataType(int raw) { dt.raw = raw; }
      inline DataType(const char* sym) { memcpy(dt.sym, sym, 4); }
    };
  
    enum class FormatType : char {
      none = 0,
      number = 1,
      string = 2,
      array = 3
    };
  
    FloatVar() = default;
    FloatVar(const char* name     , DataType data_type);
    FloatVar(const char* string  , const char* name     , DataType data_type);
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    FloatVar(T number, const char* name  , DataType data_type);
    FloatVar(const FloatVar* list, halfuint list_count  , const char* name  , DataType data_type);
    // intependant
    inline FloatVar(const FloatVar& other) { copyother(other); }
    // steals identity
    inline FloatVar(FloatVar&& other) { moveother(std::move(other)); }
    
    inline bool          isArray()   const { return mLength.type == FormatType::array; }
    inline bool          isString()  const { return mLength.type == FormatType::string; }
    inline bool          isNumber()  const { return mLength.type == FormatType::number; }
    inline halfuint      getSize()   const { return mLength.length; }
    inline const char*   getName()   const { return maName; }
    inline DataType      getType()   const { return mDataType; }
    inline FormatType    getFormat() const { return mLength.type; }
    inline const char*   getString() const { return mData.aString; }
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value,T>::type getNumber() const;
    
    void clear();
    inline void reserve(halfuint new_capacity);
    inline void resize(halfuint new_length);
    inline void shrinkToFit();
    void push(const FloatVar& that);
    void push(char that);
    void embrace(FloatVar&& that);

    inline FloatVar& operator[] (halfuint index);
    // operator [] here should be made

    inline void operator=(const FloatVar& other) { copyother(other); }
    inline void operator=(FloatVar&& other) { moveother(std::move(other)); }
           bool operator==(const FloatVar& other) const;

    ~FloatVar();
    
  private:
    inline void assignname(const char* name);
    void        copyother(const FloatVar& other);
    void        moveother(FloatVar&& other);
    void        reallocate(halfuint new_length);
  
    char* maName = nullptr;
    union DataCapsula {
      length_t  number;
      char*     aString;
      FloatVar* aList;
    } mData;
    
    DataType mDataType;
    
    struct LengthStruct {
      halfuint length;
      halfuint capacity;
      FormatType type;
    } mLength = {0};
  };

  template<typename T, typename>
  FloatVar::FloatVar(T number, const char* name, DataType data_type) {
    assignname(name);
    mLength.type = FormatType::number;

    mLength.length = sizeof(T);
    mLength.capacity = sizeof(T);

    mData.number = 0;
    memcpy(&mData.number, (void*)(&number), sizeof(T));

    mDataType = data_type;
  }

  template<typename T>
  typename std::enable_if<std::is_arithmetic<T>::value, T>::type FloatVar::getNumber() const {
    if (isNumber())
      return *((T*)&mData.number);
    else
      return 0;
  }
}

