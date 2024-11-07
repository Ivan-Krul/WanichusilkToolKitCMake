
// Check windows
#if _WIN32 || _WIN64
#if _WIN64
#define BIT64
#else
#define BIT32
#endif
#endif

// Check GCC
#if __GNUC__
#if __x86_64__ || __ppc64__
#define BIT64
#else
#define BIT32
#endif
#endif

#ifdef BIT64
typedef int halfint;
typedef short quadint;

typedef unsigned int halfuint;
typedef unsigned short quaduint;
#elif BIT32
typedef short quadint;
typedef char halfint;

typedef unsigned short quaduint;
typedef unsigned char halfuint;
#endif

