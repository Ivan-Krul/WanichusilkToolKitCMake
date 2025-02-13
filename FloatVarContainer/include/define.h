#include <stddef.h>

// Check windows
#if _WIN32 || _WIN64
  #if _WIN64
  #define CPU_BIT64
  #else
  #define CPU_BIT32
  #endif
#endif

// Check GCC
#if __GNUC__
  #if __x86_64__ || __ppc64__
  #define CPU_BIT64
  #else
  #define CPU_BIT32
  #endif
#endif

typedef ptrdiff_t length_t;

#ifdef CPU_BIT64
typedef int halfint;
typedef short quadint;

typedef unsigned int halfuint;
typedef unsigned short quaduint;

#define MAX_BITS 64

#else
  #ifdef CPU_BIT32
  typedef short halfint;
  typedef char quadint;

  typedef unsigned short quaduint;
  typedef unsigned char halfuint;

  #define MAX_BITS 32

  #else
  #error we got a wild architecture here
  #endif
#endif

// better to have twice as bigger range sacrificing 1 number
#define AVOID_MAX(x) (~x==0?x-1:x)


#if __cplusplus >= 202002L
#include <bit>          // Include <bit> for std::bit_cast
#define USE_BIT_CAST
#endif


// flags for compilation guards in FloatVar
#define FLOAT_VAR_INDEX_GUARD
#define FLOAT_VAR_ITERATOR_TYPE_GUARD
#define FLOAT_VAR_KEEP_64BIT_NUMBER
#define FLOAT_VAR_KEEP_CPUBIT_NUMBER

#ifdef FLOAT_VAR_KEEP_64BIT_NUMBER
typedef long long fvnumber_t;
#else
  #ifdef FLOAT_VAR_KEEP_CPUBIT_NUMBER
  typedef length_t fvnumber_t;
  #else
  typedef int fvnumber_t;
  #endif
#endif

#ifdef FLOAT_VAR_INDEX_GUARD
#define FV_ARAY_ACCESS(a,i,len) (i>=len?a[0]:a[i])
#else
#define FV_ARAY_ACCESS(a,i,l) (a[i])
#endif

#ifdef FLOAT_VAR_ITERATOR_TYPE_GUARD
#define FV_MOVE_PTR_ITERATOR(a, is_same, is_begin, len) (is_same?(is_begin?a:a + len):(a))
#define FV_MOVE_PTR_ITERATOR_REVERSE(a, is_same, is_begin, len) (is_same?(is_begin?a + (len - 1):a - 1):(a))
#else
#define FV_MOVE_PTR_ITERATOR(a, is_same, is_begin, len) (is_begin?a:a + len)
#define FV_MOVE_PTR_ITERATOR_REVERSE(a, is_same, is_begin, len) (is_begin?a + (len - 1):a - 1)
#endif
