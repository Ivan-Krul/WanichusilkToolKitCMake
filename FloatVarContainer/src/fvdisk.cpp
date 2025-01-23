#include "../include/fvdisk.h"

#include <fstream>
#include <sys/stat.h>

namespace FVC {
  bool FloatVarDisk::save(const char* filepath) {
    std::ofstream fout(filepath, std::ios::binary);

    if (!fout.good()) {
      maError = "file isn't wanting to be opened";
      return true;
    }

    fout.write("FloatVar", 8);
    saveFloatVar(mBuf, fout);

    fout.close();

    return false;
  }

  bool FloatVarDisk::load(const char* filepath) {
    std::ifstream fin(filepath, std::ios::binary);

    if (!fin.is_open()) {
      maError = "file isn't exist or isn't wanting to be opened";
      return true;
    }

    {
      char* buf;

      fin.read(buf, 8);
      if (strcmp(buf, "FloatVar") != 0) {
        maError = "magic value isn't exist";
        return true;
      }
    }

    if (loadFloatVar(fin)) return true;

    fin.close();
    return false;
  }

  void FloatVarDisk::saveFloatVar(const FloatVar& fv, std::ofstream& fout) {

    length_t number = strlen(fv.getName());

    fout.write((char*)&number, namelen_bytes);
    fout.write(fv.getName(), number);

    number = (length_t)fv.getFormat();

    fout.write(fv.getType().dt.sym, 4);
    fout.write((char*)&number, sizeof(fv.getFormat()));

    if (fv.isList()) {
      number = fv.getSize();
      fout.write((char*)&number, arraylen_bytes);

      for (length_t i = 0; i < fv.getSize(); i++)
        saveFloatVar(fv.at(i), fout);
    }
    else if (fv.isString()) {
      number = fv.getSize();
      fout.write((char*)&number, arraylen_bytes);
      fout.write(fv.getString(), number);
    }
    else if (fv.isNumber()) {
      number = fv.getNumber<length_t>();
      fout.write((char*)&number, 8);
    }
  }

  bool FloatVarDisk::loadFloatVar(std::ifstream& fin) {
    char* buf;

    // work there ig

    return false;
  }
}

