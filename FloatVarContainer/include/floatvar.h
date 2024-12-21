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
    union DataType {
      int raw;
      char sym[4];
    };
  
    enum class FormatType : char {
      none = 0,
      number = 1,
      string = 2,
      array = 3
    };
  
    FloatVar();
    FloatVar(const char* name     , DataType data_type);
    FloatVar(const char* string  , const char* name     , DataType data_type);
    
    template<typename T>
    typename std::enable_if<std::is_arithmetic<T>::value>::type FloatVar(T number  , halfuint binary_count, const char* name  , DataType data_type);
    
    FloatVar(const FloatVar* list, halfuint list_count  , const char* name  , DataType data_type);
    
    inline FloatVar(const FloatVar& other);
    
    inline FormatType    getFormat() const;
    
    inline bool          isArray()   const;
    inline bool          isString()  const;
    inline bool          isNumber()  const;
    inline halfuint      getSize()   const;
    inline const char*   getName()   const;
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
    
    DataType mDataType = { .raw = gDefaultDataType };
    
    struct LengthStruct {
      halfuint length;
      halfuint capacity : ((MAX_BITS >> 1) - 2);
      FormatType type  : 2;
    } mLength = {0};
    
  };
}

