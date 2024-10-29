
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
using halfint = int;
#elif BIT32
using halfint = short;
#endif

#ifdef BIT64
using quadint = short;
#elif BIT32
using quadint = char;
#endif
