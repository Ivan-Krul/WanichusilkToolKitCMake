#pragma once
#include <stdint.h>

#include "darray.h"

namespace FVC {
  constexpr char gDefaultDataType[] = "void"
  
  struct FloatVar {
    FloatVar() noexcept;
    
    
    char* name;
    size_t name_length;
    
    char data_type[4];

    _smallarray<FloatVar> list;
    
    char* binary;
    size_t binary_length;
  };
}

