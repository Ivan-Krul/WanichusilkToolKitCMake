#include "floatvar.h"

namespace FVC {
  FloatVar::FloatVar() noexcept {
    memcpy(data_type, gDefaultDataType, 4);    
  }

}
