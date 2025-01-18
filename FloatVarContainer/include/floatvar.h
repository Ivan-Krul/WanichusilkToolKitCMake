#pragma once
#include <stdint.h>
#include <string.h>
#include <memory>
#include <type_traits>

#include "define.h"

namespace FVC {
  constexpr int gDefaultDataType = 0x766F6964; // void
  
  class FloatVar {
  public:
    typedef int DataType;
  
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
    
    inline FloatVar(const FloatVar& other);
    
    inline FormatType    getFormat() const { return mLength.type; }
    inline bool          isArray()   const { return mLength.type == FormatType::array; }
    inline bool          isString()  const { return mLength.type == FormatType::string; }
    inline bool          isNumber()  const { return mLength.type == FormatType::number; }
    inline halfuint      getSize()   const { return mLength.length; }
    inline const char*   getName()   const { return maName; }
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value,T>::type getNumber() const;
    
    ~FloatVar();
    
  private:
    inline void ctol(halfuint& cap);
    inline void ltoc(halfuint& len);
  
    inline void assignname(const char* name);
  
    char* maName;
    union DataCapsula {
      length_t  number;
      char*     aString;
      FloatVar* aList;
    } mData;
    
    DataType mDataType;
    
    struct LengthStruct {
      halfuint length;
      halfuint capacity : ((MAX_BITS >> 1) - 2);
      FormatType type  : 2;
    } mLength = {0};
    
  };

  template<typename T, typename>
  FloatVar::FloatVar(T number, const char* name, DataType data_type) {
    assignname(name);
    mLength.type = FormatType::number;

    mLength.length = sizeof(T);

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

