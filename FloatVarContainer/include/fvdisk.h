#pragma once
#include "floatvar.h"

namespace FVC {
  class FloatVarDisk {
    constexpr int namelen_bytes = 1;
    constexpr int arraylen_bytes = 4;

  public:
    inline void insertFloatVar(const FloatVar& fv) { mBuf = fv; }
    bool save(const char* filepath);
    inline const char* pullError() const { return maError; }
    inline FloatVar&& pullFloatVar() { return mBuf; }

  private:
    void saveFloatVar(const FloatVar& fv, std::ofstream& fout);

    FloatVar mBuf;
    const char* maError;
  }
}

