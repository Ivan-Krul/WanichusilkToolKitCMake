#pragma once
#include "fvheader.h"
#include "define.h"

namespace FVC {
  class FloatVarNumber : public FloatVarHeader {
  public:
    FloatVarNumber() = default;
    template<typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
    explicit FloatVarNumber(T number, const char* name, DataType data_type);
    // intependant
    inline explicit FloatVarNumber(const FloatVarNumber& other) : FloatVarHeader(other) { copyother(other); }
    // steals identity
    inline explicit FloatVarNumber(FloatVarNumber&& other) noexcept : FloatVarHeader(other) { moveotherheader(std::move(other)); }

    template<typename T>
    inline typename std::enable_if_t<std::is_arithmetic<T>::value, T> getNumber() const;

    template<typename T>
    inline typename std::enable_if_t<std::is_arithmetic<T>::value> operator=(T number);

  private:
    void setnumbersize(quadint size);
    void copyother(const FloatVarNumber& other);
    void moveother(FloatVarNumber&& other); // yeee

    length_t mNumber = 0;

  };

  template<typename T, typename>
  FloatVarNumber::FloatVarNumber(T number, const char* name, DataType data_type) : FloatVarHeader(name, data_type, FormatType::number) {
    memcpy(&mNumber, &number, sizeof(T));
    setnumbersize(sizeof(T));
  }

  template<typename T>
  typename std::enable_if_t<std::is_arithmetic<T>::value, T> FloatVarNumber::getNumber() const {
    return *((T*)&mData.number);
  }

  template<typename T>
  inline typename std::enable_if_t<std::is_arithmetic<T>::value> FloatVarNumber::operator=(T number) {
    mNumber = 0;
    memcpy(&mNumber, &number, sizeof(T));
    setnumbersize(sizeof(T));
  }

}
