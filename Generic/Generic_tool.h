#define likely(x)   __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

#ifndef LTT_inline
    #ifdef _MSC_VER
        #define LTT_inline __inline
    #else
        #define LTT_inline inline
    #endif
#endif /* LTT_inline */

#ifndef LTT_unused
    #if (defined __clang__ && __clang_major__ >= 3) || (defined __GNUC__ && __GNUC__ >= 3)
        #define LTT_unused __attribute__((__unused__))
    #else
        #define LTT_unused
    #endif
#endif /* LTT_unused */