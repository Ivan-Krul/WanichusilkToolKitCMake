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

  void FloatVarDisk::saveFloatVarHeader(const FloatVar& fv, std::ofstream& fout) {
    length_t number = strlen(fv.getName());

    fout.write((char*)&number, namelen_bytes);
    fout.write(fv.getName(), number);

    number = fv.getType().dt.raw;
    fout.write((char*)&number, 4);

    number = (length_t)fv.getFormat();
    fout.write((char*)&number, sizeof(fv.getFormat()));
  }

  void FloatVarDisk::saveFloatVar(const FloatVar& fv, std::ofstream& fout) {
    saveFloatVarHeader(fv, fout);

    length_t number = strlen(fv.getName());

    switch (fv.getFormat()) {
    case FloatVar::FormatType::list:
      number = fv.getSize();
      fout.write((char*)&number, arraylen_bytes);

      for (length_t i = 0; i < fv.getSize(); i++)
        saveFloatVar(fv.at(i), fout);
      break;
    case FloatVar::FormatType::array:
      number = fv.getOffset();
      fout.write((char*)&number, offsetlen_bytes);
      number = fv.getSize();
      fout.write((char*)&number, arraylen_bytes);

      fout.write((const char*)fv.getRawData(), number * fv.getOffset());
      break;
    case FloatVar::FormatType::string:
      number = fv.getSize();
      fout.write((char*)&number, arraylen_bytes);
      fout.write(fv.getString(), number);
      break;
    case FloatVar::FormatType::number:
      number = fv.getNumber<length_t>();
      fout.write((char*)&number, 8);
      break;
    }
  }

  FloatVar FloatVarDisk::loadFloatVarHeader(std::ifstream& fin) {
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

    delete[] buf;
    return fv;
  }

  FloatVar FloatVarDisk::loadFloatVar(std::ifstream& fin) {
    FloatVar fv = std::move(loadFloatVarHeader(fin));
    char* buf;
    length_t number = 0;

    switch (fv.getFormat()) {
    case FloatVar::FormatType::list:
      fin.read((char*)&number, arraylen_bytes);

      fv.reserve(number);

      for (length_t i = 0; i < fv.getCapacity(); i++)
        fv.emplace(loadFloatVar(fin));
      break;
    case FloatVar::FormatType::array:
      fin.read((char*)&number, offsetlen_bytes);
      fv.reoffset(number);
      fin.read((char*)&number, offsetlen_bytes);
      fv.resize(number);

      buf = new char[number * fv.getOffset()];

      fin.read(buf, number * fv.getOffset());

      fv = buf; // feeling bad

      delete[] buf;
      break;
    case FloatVar::FormatType::string:
      fin.read((char*)&number, arraylen_bytes);
      buf = new char[number + 1];
      buf[number] = 0;
      fin.read(buf, number);

      fv = buf;

      delete[] buf;
      break;
    case FloatVar::FormatType::number:
      fin.read((char*)&number, 8);

      fv = number;
      break;
    }

    return fv;
  }
}

