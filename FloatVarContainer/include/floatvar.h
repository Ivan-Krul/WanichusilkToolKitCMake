#pragma once
#include <stdint.h>

#include "define.h"

namespace FVC {
  constexpr int gDefaultDataType = 'void';
  
  class FloatVar {
  public:
    union DataType {
      int raw;
      char sym[4];
    };
  
    enum class FormatType {
      none = 0,
      number = 1,
      string = 2,
      array = 3
    };
  
    FloatVar() noexcept;
    FloatVar(const char* binary  , const char* name     , DataType data_type);
    FloatVar(const void* binary  , length_t binary_count, const char* name  , DataType data_type);
    FloatVar(const FloatVar* list, length_t list_count  , const char* name  , DataType data_type) noexcept;
    
    
    inline bool     isArray() noexcept const;
    inline length_t getSize() noexcept const;
    
    ~FloatVar() noexcept;
    
  private:
    char* maName;
    union DataCapsula {
      
      char*     aBinary;
      FloatVar* aList;
    } mData;
    
    DataType mDataType;
    
    uint32_t mNameLength = 0;
    
    struct LengthStruct {
      length_t length : MAX_BITS - 2;
      FormatType type : 2;
    } mLength = 0;
    

    
  };
}

