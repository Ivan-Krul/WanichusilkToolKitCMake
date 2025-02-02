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
    std::ifstream fin(filepath, std::ios::binary| std::ios::in);

    if (!fin.is_open()) {
      maError = "file isn't exist or isn't wanting to be opened";
      return true;
    }

    {
      char* buf = new char[9];
      buf[8] = 0;
      fin.read(buf, 8);
      if (strcmp(buf, "FloatVar") != 0) {
        maError = "magic value isn't exist";
        delete[] buf;
        return true;
      }

      delete[] buf;
    }

    mBuf = loadFloatVar(fin);

    fin.close();
    return false;
  }

  void FloatVarDisk::saveFloatVar(const FloatVar& fv, std::ofstream& fout) {

    length_t number = strlen(fv.getName());

    fout.write((char*)&number, namelen_bytes);
    fout.write(fv.getName(), number);

    number = fv.getType().dt.raw;
    fout.write((char*)&number, 4);

    number = (length_t)fv.getFormat();
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

  FloatVar FloatVarDisk::loadFloatVar(std::ifstream& fin) {
    FloatVar fv;
    char* buf;
    length_t number = 0;

    fin.read((char*)&number, namelen_bytes);
    buf = new char[number + 1];
    buf[number] = 0;
    fin.read(buf, number);

    fv.rename(buf);
    number = 0;
    delete[] buf;
    buf = new char[4];
    fin.read(buf, 4);
    fin.read((char*)&number, sizeof(fv.getFormat()));

    fv.retype(buf);
    fv.reformat((FloatVar::FormatType)number);
    number = 0;
    if (fv.isList()) {
      fin.read((char*)&number, arraylen_bytes);

      fv.reserve(number);

      for (length_t i = 0; i < fv.getCapacity(); i++)
        fv.emplace(loadFloatVar(fin));
    }
    else if (fv.isString()) {
      fin.read((char*)&number, arraylen_bytes);
      delete[] buf;
      buf = new char[number + 1];
      buf[number] = 0;
      fin.read(buf, number);
      
      fv = buf;
    }
    else if (fv.isNumber()) {
      fin.read((char*)&number, 8);

      fv = number;
    }

    delete[] buf;
    return fv;
  }
}

