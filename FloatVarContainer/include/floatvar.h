#pragma once
#include <stdint.h>

namespace FVC {
  constexpr char gDefaultDataType[] = "void";
  
  struct FloatVar {
    
    char* name;
    size_t name_length;
    
    char data_type[4];

    struct FloatVar_List {
      FloatVar* lists;
      size_t list_count;
    } list;
    
    char* binary;
    size_t binary_length;
  };
}

