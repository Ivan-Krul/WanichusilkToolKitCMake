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


#elif CPU_BIT32
typedef short halfint;
typedef char quadint;

typedef unsigned short quaduint;
typedef unsigned char halfuint;

#define MAX_BITS 32


#endif

// better to have twice as bigger range sacrificing 1 number
#define AVOID_MAX(x) (~x==0?x-1:x)