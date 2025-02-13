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
    inline explicit FloatVarNumber(FloatVarNumber&& other) noexcept : FloatVarHeader(other) { moveother(std::move(other)); }

    template<typename T>
    inline typename std::enable_if_t<std::is_arithmetic<T>::value, T> getNumber() const;

    template<typename T>
    inline typename std::enable_if_t<std::is_arithmetic<T>::value> operator=(T number);

    inline quaduint size() const { return mState.params[0]; }

  private:
    void copyother(const FloatVarNumber& other);
    void moveother(FloatVarNumber&& other);

    fvnumber_t mNumber;
  };

  template<typename T, typename>
  FloatVarNumber::FloatVarNumber(T number, const char* name, DataType data_type) : FloatVarHeader(name, data_type, FormatType::number) {
    std::memcpy(&mNumber, &number, sizeof(T));
    mState.param[0] = (sizeof(T));
  }

  template<typename T>
  typename std::enable_if_t<std::is_arithmetic<T>::value, T> FloatVarNumber::getNumber() const {
    return *((T*)&mData.number);
  }

  template<typename T>
  inline typename std::enable_if_t<std::is_arithmetic<T>::value> FloatVarNumber::operator=(T number) {
    if (sizeof(T) != mState.params[0]) {
      std::memcpy(&mNumber, &number, sizeof(T));
      mState.params[0] = sizeof(T);
      return;
    }

#ifdef USE_BIT_CAST
    mNumber = std::bit_cast<fvnumber_t>(number);
#else
    std::memcpy(&mNumber, &number, sizeof(T));
#endif
  }

}
