#include "../include/fvnumber.h"

namespace FVC {
  void FloatVarNumber::copyother(const FloatVarNumber& other) {
    copyotherheader(other);

    mNumber = other.mNumber;
  }

  void FloatVarNumber::moveother(FloatVarNumber&& other) {
    mNumber = other.mNumber;

    moveotherheader(std::move(other));
  }
}
