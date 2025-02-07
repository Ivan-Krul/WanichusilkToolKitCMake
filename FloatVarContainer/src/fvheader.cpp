#include "../include/fvheader.h"

namespace FVC {
  void FloatVarHeader::assignname(const char* name) {
    if (mName.heapname && mState.isnameinheap) delete[] mName.heapname;

    length_t len = strlen(name);
    mState.isnameinheap = len < sizeof(length_t);

    if (mState.isnameinheap) {
      memcpy(&mName.stackname, name, len);
      mName.stackname[len] = 0;
    }
    else {
      mName.heapname = new char[len + 1];
      memcpy(mName.heapname, name, len);
      mName.heapname[len] = 0;
    }
  }

  void FloatVarHeader::copyotherheader(const FloatVarHeader& other) {
    assignname(other.getName());
    mDataType = other.getType();
    mState.format = other.getFormat();
  }

  void FloatVarHeader::moveotherheader(FloatVarHeader&& other) {
    mName.heapname = other.mName.heapname;
    mDataType = other.mDataType;
    mState = other.mState;
    
    other.mName.heapname = nullptr;
  }
}
