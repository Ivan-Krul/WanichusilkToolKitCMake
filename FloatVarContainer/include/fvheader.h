#pragma once 
#include <memory>

#include "define.h"

namespace FVC {
  constexpr int gDefaultDataType = 0x766F6964; // void

  class FloatVarHeader {
  public:
    struct DataType {
      union uDataType {
        char sym[4];
        int raw;
      } dt;
      inline DataType() { dt.raw = gDefaultDataType; }
      inline DataType(int raw) { dt.raw = raw; }
      inline DataType(const char* sym) { dt.raw = *(reinterpret_cast<const int*>(sym)); }
    };

    enum class FormatType : char {
      none = 0,
      number = 1,
      string = 2,
      array = 3,
      list = 4
    };

  public:
    inline FloatVarHeader() = default;
    inline FloatVarHeader(const char* name, DataType dt) : mDataType(dt) { assignname(name); }
    inline FloatVarHeader(const char* name, DataType dt, FormatType ft) : mDataType(dt) {
      assignname(name);
      mState.format = ft;
    }

    // intependant
    inline FloatVarHeader(const FloatVarHeader& other) { copyotherheader(other); }
    // steals identity
    inline FloatVarHeader(FloatVarHeader&& other) noexcept { moveotherheader(std::move(other)); }

    inline bool          isList()   const { return mState.format == FormatType::list; }
    inline bool          isArray()  const { return mState.format == FormatType::array; }
    inline bool          isString() const { return mState.format == FormatType::string; }
    inline bool          isNumber() const { return mState.format == FormatType::number; }
    inline bool          isNone()   const { return mState.format == FormatType::none; }

    inline FormatType  getFormat() const { return mState.format; }
    inline DataType    getType()   const { return mDataType; }
    inline const char* getName()   const { return mState.isnameinheap ? mName.heapname : mName.stackname; }

    virtual void clear() {}

    inline void reformat(FormatType format) { clear(); mState.format = format; }
    inline void retype(DataType dt)         { mDataType = dt; }
    inline void rename(const char* name)    { assignname(name); }

    inline void operator= (const FloatVarHeader& other) { copyotherheader(other); }
    inline void operator= (FloatVarHeader&& other) noexcept { moveotherheader(std::move(other)); }

    virtual ~FloatVarHeader() { if (mState.isnameinheap && mName.heapname) delete[] mName.heapname; }


  protected:
    inline void assignname(const char* name);
    inline void copyotherheader(const FloatVarHeader& other);
    inline void moveotherheader(FloatVarHeader&& other);

  private:
    union Name {
      char* heapname = nullptr;
      char stackname[8];
    } mName;

    DataType mDataType;

  protected:
    struct State {
      FormatType format : 4;
      bool isnameinheap : 1;
      char params : 3;
    } mState = { 0 };
  };
}
