#pragma once
#include <stdint.h>
#include <string.h>
#include <memory>
#include <type_traits>
#include <initializer_list>

#include "define.h"

#define FLOAT_VAR_INDEX_GUARD
#define FLOAT_VAR_ITERATOR_TYPE_GUARD

#ifdef FLOAT_VAR_INDEX_GUARD
#define ARAY_ACCESS(a,i,m) (i>=m?a[0]:a[i])
#else
#define ARAY_ACCESS(a,i,m) (a[i])
#endif

#ifdef FLOAT_VAR_ITERATOR_TYPE_GUARD
#define MOVE_PTR_ITERATOR(a, is_same, is_begin, len) (is_same?(is_begin?a:a + len):(a))
#define MOVE_PTR_ITERATOR_REVERSE(a, is_same, is_begin, len) (is_same?(is_begin?a + (len - 1):a - 1):(a))
#else
#define MOVE_PTR_ITERATOR(a, is_same, is_begin, len) (is_begin?a:a + len)
#define MOVE_PTR_ITERATOR_REVERSE(a, is_same, is_begin, len) (is_begin?a + (len - 1):a - 1)
#endif


namespace FVC {
  constexpr int gDefaultDataType = 0x766F6964; // void
  
  class FloatVar {
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
  
    FloatVar() = default;
    explicit FloatVar(const char* name     , DataType data_type);
    explicit FloatVar(const char* string  , const char* name     , DataType data_type);
    template<typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
    explicit FloatVar(T number, const char* name  , DataType data_type);
    template<typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
    explicit FloatVar(T* numbers, halfuint list_count, const char* name, DataType data_type);
    template<typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
    explicit FloatVar(std::initializer_list<T> list, const char* name, DataType data_type);
    explicit FloatVar(const FloatVar* list, halfuint list_count, const char* name  , DataType data_type);
    // intependant
    inline FloatVar(const FloatVar& other) { copyother(other); }
    // steals identity
    inline FloatVar(FloatVar&& other) noexcept { moveother(std::move(other)); }
    
    inline bool          isList()      const { return mLength.type == FormatType::list; }
    inline bool          isArray()     const { return mLength.type == FormatType::array; }
    inline bool          isString()    const { return mLength.type == FormatType::string; }
    inline bool          isNumber()    const { return mLength.type == FormatType::number; }
    inline bool          isNone()      const { return mLength.type == FormatType::none; }
    inline halfuint      getSize()     const { return mLength.length; }
    inline halfuint      getCapacity() const { return mLength.capacity; }
    inline halfuint      getOffset()   const { return mLength.offset; }
    inline const void const* getRawData() const { return mData.aString; }
    inline const char*   getName()     const { return maName; }
    inline DataType      getType()     const { return mDataType; }
    inline FormatType    getFormat()   const { return mLength.type; }
    inline const char*   getString()   const { return mData.aString; }
    template<typename T>
    inline typename std::enable_if_t<std::is_arithmetic<T>::value, T> getNumber() const;
    
    void clear();
    void reserve(halfuint new_capacity);
    void resize(halfuint new_length);
    inline void shrinkToFit();
    void push(const FloatVar& that);

    template <typename T>
    typename std::enable_if_t<std::is_arithmetic<T>::value> push(T that);
    void push(char that);
    void emplace(FloatVar&& that);

    inline FloatVar* begin()  { return MOVE_PTR_ITERATOR(mData.aList, isList(), true, mLength.length); }
    inline FloatVar* end()    { return MOVE_PTR_ITERATOR(mData.aList, isList(), false, mLength.length); }
    inline FloatVar* rbegin() { return MOVE_PTR_ITERATOR_REVERSE(mData.aList, isList(), true, mLength.length); }
    inline FloatVar* rend()   { return MOVE_PTR_ITERATOR_REVERSE(mData.aList, isList(), false, mLength.length); }
    inline const FloatVar* cbegin()  const { return MOVE_PTR_ITERATOR(mData.aList, isList(), true, mLength.length); }
    inline const FloatVar* cend()    const { return MOVE_PTR_ITERATOR(mData.aList, isList(), false, mLength.length); }
    inline const FloatVar* crbegin() const { return MOVE_PTR_ITERATOR_REVERSE(mData.aList, isList(), true, mLength.length); }
    inline const FloatVar* crend()   const { return MOVE_PTR_ITERATOR_REVERSE(mData.aList, isList(), false, mLength.length); }

    inline FloatVar& operator[] (halfuint index);
           const FloatVar& at(halfuint index) const;

    inline char& strIndex(halfuint index);
    inline const char strIndexAt(halfuint index) const;

    template<typename T>
    inline typename std::enable_if_t<std::is_arithmetic<T>::value, T&> numIndex(halfuint index);
    template<typename T>
    inline typename std::enable_if_t<std::is_arithmetic<T>::value, const T> numIndexAt(halfuint index) const;

    inline void rename(const char* name) { assignname(name); }
    inline void reformat(FormatType format) { clear(); mLength.type = format; }
    inline void reoffset(quaduint offset) { mLength.offset = offset; }
    inline void retype(DataType dt) { mDataType = dt; }

    template<typename T>
    inline typename std::enable_if_t<std::is_arithmetic<T>::value> operator=(T number);

           void operator=(const char* str);
           void restring(char* str, halfuint len);

    inline void operator=(const FloatVar& other) { copyother(other); }
    inline void operator=(FloatVar&& other) noexcept { moveother(std::move(other)); }
           bool operator==(const FloatVar& other) const;

    ~FloatVar();
    
  private:
    inline void assignname(const char* name);
    inline void copystring(char* str);
    void        copyother(const FloatVar& other);
    void        moveother(FloatVar&& other);
    inline void listexpansioncheck();
    void        reallocate(halfuint new_length);
  
    char* maName = nullptr;
    union DataCapsula {
      length_t  number = 0;
      char*     aString;
      length_t* aArray;
      FloatVar* aList;
    } mData;
    
    DataType mDataType;
    
    struct LengthStruct {
      halfuint length;
      halfuint capacity;
      quadint offset;
      FormatType type;
    } mLength = {0};
  };

  template<typename T, typename>
  FloatVar::FloatVar(T number, const char* name, DataType data_type) {
    assignname(name);

    mLength = { sizeof(T), sizeof(T), 0, FormatType::number };

    mData.number = 0;
    memcpy(&mData.number, &number, sizeof(T));

    mDataType = data_type;
  }

  template<typename T, typename>
  inline FloatVar::FloatVar(T* numbers, halfuint list_count, const char* name, DataType data_type) {
    assignname(name);
    mLength = {list_count, list_count, sizeof(T), FormatType::array};
    
    mData.aString = new char[mLength.length * sizeof(T)];
    memcpy(mData.aArray, numbers, mLength.length * sizeof(T));

    mDataType = data_type;
  }

  template<typename T, typename>
  FloatVar::FloatVar(std::initializer_list<T> list, const char* name, DataType data_type) {
    assignname(name);
    mLength = { list.size(), list.size(), sizeof(T), FormatType::array };

    mData.aString = new char[mLength.length * sizeof(T)];
    memcpy(mData.aArray, list.begin(), mLength.length * sizeof(T));

    mDataType = data_type;
  }

  template<typename T>
  typename std::enable_if_t<std::is_arithmetic<T>::value, T> FloatVar::getNumber() const {
    return isNumber() ? *((T*)&mData.number) : 0;
  }

  template<typename T>
  typename std::enable_if_t<std::is_arithmetic<T>::value> FloatVar::push(T that) {
    if (mLength.type != FormatType::array) return;
    if (sizeof(T) != mLength.offset) return;
    listexpansioncheck();

    ((T*)mData.aArray)[mLength.length] = that;

    mLength.length++;
  }

  template<typename T>
  inline typename std::enable_if_t<std::is_arithmetic<T>::value, T&> FloatVar::numIndex(halfuint index) {
    return ARAY_ACCESS(((T*)mData.aArray), index, mLength.length);
  }

  template<typename T>
  inline typename std::enable_if_t<std::is_arithmetic<T>::value, const T> FloatVar::numIndexAt(halfuint index) const {
    return ARAY_ACCESS(((T*)mData.aArray), index, mLength.length);
  }

  template<typename T>
  inline typename std::enable_if_t<std::is_arithmetic<T>::value> FloatVar::operator=(T number) {
    if (mLength.type != FormatType::number) return;

    mData.number = 0;
    memcpy(&mData.number, &number, sizeof(T));
    mLength.length = sizeof(T);
    mLength.capacity = sizeof(T);
  }
}

