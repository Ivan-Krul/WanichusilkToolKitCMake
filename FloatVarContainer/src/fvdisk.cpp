#include "../include/fvdisk.h"

#include <fstream>
#include <sys/stat.h>

namespace FVC {
  FloatVar FloatVarDisk::save(const char* filepath) {
    std::ofstream fout(filepath, std::ios::binary);

    if (!fout.is_open()) {
      maError = "file don't want to be opened";
      return true;
    }

    fout.write("FloatVar", 8);
    saveFloatVar(mBuf, fout);

    return false;
  }
  void FloatVarDisk::saveFloatVar(const FloatVar& fv, std::ofstream& fout) {

    length_t number = strlen(fv.getName());

    fout.write(&number, namelen_bytes);
    fout.write(fv.getName(), number);

    number = fv.getFormat();

    fout.write(fv.getType().dt.sym, 4);
    fout.write(&number, sizeof(fv.getFormat()));

    if (fv.isList()) {
      number = fv.getSize();
      fout.write(&number, arraylen_bytes);

      for (length_t i = 0; i < fv.getSize(); i++)
        saveFloatVar(fv.at(i), fout);
    }
    else if (fv.isString()) {
      number = fv.getSize();
      fout.write(&number, arraylen_bytes);
      fout.write(fv.getString(), number);
    }
    else {
      number = fv.getNumber<length_t>();
      fout.write(number, 8);
    }
  }
}

