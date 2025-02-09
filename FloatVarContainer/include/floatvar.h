#pragma once
#include <stdint.h>
#include <string.h>
#include <memory>
#include <type_traits>
#include <initializer_list>

#include "define.h"
#include "fvheader.h"


namespace FVC {
  class FloatVar : public FloatVarHeader {
  public:
  
    FloatVar() = default;
    explicit FloatVar(const char* name, DataType data_type) : FloatVarHeader(name, data_type) {}
    explicit FloatVar(const char* string  , const char* name     , DataType data_type);
    template<typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
    explicit FloatVar(T* numbers, halfuint list_count, const char* name, DataType data_type);
    template<typename T, typename = typename std::enable_if_t<std::is_arithmetic<T>::value>>
    explicit FloatVar(std::initializer_list<T> list, const char* name, DataType data_type);
    explicit FloatVar(const FloatVar* list, halfuint list_count, const char* name  , DataType data_type);
    // intependant
    inline FloatVar(const FloatVar& other) : FloatVarHeader(other) { copyother(other); }
    // steals identity
    inline FloatVar(FloatVar&& other) noexcept : FloatVarHeader(std::move(other)) { moveother(std::move(other)); }
    
    inline halfuint      getSize()     const { return mLength.length; }
    inline halfuint      getCapacity() const { return mLength.capacity; }
    inline halfuint      getOffset()   const { return mLength.offset; }
    inline const void const* getRawData() const { return mData.aString; }

    inline const char*   getString()   const { return mData.aString; }
    
    void clear() override;
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


    inline void reoffset(quaduint offset) { mLength.offset = offset; }

           void operator=(const char* str);
           void restring(char* str, halfuint len);

    inline void operator=(const FloatVar& other) { copyother(other); }
    inline void operator=(FloatVar&& other) noexcept { moveother(std::move(other)); }
           bool operator==(const FloatVar& other) const;

    virtual ~FloatVar();
    
  private:
    inline void copystring(char* str);
    void        copyother(const FloatVar& other);
    void        moveother(FloatVar&& other);
    inline void listexpansioncheck();
    void        reallocate(halfuint new_length);
  
    union DataCapsula {
      length_t  number = 0;
      char*     aString;
      length_t* aArray;
      FloatVar* aList;
    } mData;

    struct LengthStruct {
      halfuint length;
      halfuint capacity;
      quadint offset;
    } mLength = { 0 };
  };

  template<typename T, typename>
  inline FloatVar::FloatVar(T* numbers, halfuint list_count, const char* name, DataType data_type) : FloatVarHeader(name, data_type) {
    mLength = {list_count, list_count, sizeof(T), FormatType::array};
    
    mData.aString = new char[mLength.length * sizeof(T)];
    memcpy(mData.aArray, numbers, mLength.length * sizeof(T));
  }

  template<typename T, typename>
  FloatVar::FloatVar(std::initializer_list<T> list, const char* name, DataType data_type) : FloatVarHeader(name, data_type) {
    mLength = { list.size(), list.size(), sizeof(T), FormatType::array };

    mData.aString = new char[mLength.length * sizeof(T)];
    memcpy(mData.aArray, list.begin(), mLength.length * sizeof(T));
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
}

