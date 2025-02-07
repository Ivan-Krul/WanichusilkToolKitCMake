#pragma once
#include "fvheader.h"
#include "define.h"

namespace FVC {
  class FloatVarNumber : FloatVarHeader {
  public:
    FloatVarNumber() = default;
    template<typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
    explicit FloatVarNumber(T number, const char* name, DataType data_type);

  private:
    void setnumbersize(quadint size);

    length_t mNumber = 0;


  };

  template<typename T, typename>
  FloatVarNumber::FloatVarNumber(T number, const char* name, DataType data_type) : FloatVarHeader(name, data_type, FormatType::number) {
    memcpy(&mNumber, &number, sizeof(T));
    setnumbersize(sizeof(T));
  }

}
