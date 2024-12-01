#pragma once
#include <stdint.h>
#include <string.h>
#include <memory>

#include "define.h"

namespace FVC {
  constexpr int gDefaultDataType = 'void';
  
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
  
    FloatVar() noexcept;
    FloatVar(const char* name     , DataType data_type);
    FloatVar(const char* string  , const char* name     , DataType data_type);
    
    template<typename T>
    typename std::enable_if_t<std::is_arithmetic_v<T>> FloatVar(T number  , halfuint binary_count, const char* name  , DataType data_type);
    
    FloatVar(const FloatVar* list, halfuint list_count  , const char* name  , DataType data_type) noexcept;
    
    inline FloatVar(const FloatVar& other);
    
    inline FormatType       getFormat() noexcept const;
    
    inline bool             isArray()   noexcept const;
    inline bool             isString()  noexcept const;
    inline bool             isNumber()  noexcept const;
    inline unsigned halfint getSize()   noexcept const;
    
    ~FloatVar() noexcept;
    
  private:
    inline void ctol(halfuint& cap);
    inline void ltoc(halfuint& len);
  
    inline void assignname(const char* name);
  
    char* maName;
    union DataCapsula {
      long long number;
      char*     aString;
      FloatVar* aList;
    } mData;
    
    DataType mDataType = FormatType::none;
    
    struct LengthStruct {
      halfuint length;
      halfuint capacity : ((MAX_BITS >> 1) - 2);
      FormatType type  : 2;
    } mLength = {0};
    
  };
}

