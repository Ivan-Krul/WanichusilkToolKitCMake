#pragma once
#include "floatvar.h"

namespace FVC {
  class FloatVarDisk {
    const static int namelen_bytes = 1;
    const static int arraylen_bytes = 4;

  public:
    inline void insertFloatVar(const FloatVar& fv) { mBuf = fv; }
    bool save(const char* filepath);
    bool load(const char* filepath);
    inline const char* pullError() const { return maError; }
    inline FloatVar&& pullFloatVar() { return std::move(mBuf); }

  private:
    void saveFloatVar(const FloatVar& fv, std::ofstream& fout);
    bool loadFloatVar(std::ifstream& fin);

    FloatVar mBuf;
    const char* maError;
  };
}

