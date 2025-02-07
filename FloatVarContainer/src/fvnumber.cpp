#include "../include/fvnumber.h"

namespace FVC {
  void FloatVarNumber::setnumbersize(quadint size) {
    switch (size) {
    case 1: mState.params = 0; break;
    case 2: mState.params = 1; break;
    case 4: mState.params = 2; break;
    case 8: mState.params = 3; break;
    }
  }
}
