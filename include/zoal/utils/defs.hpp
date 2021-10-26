#ifndef ZOAL_INLINE_LEVEL
#define ZOAL_INLINE_LEVEL 0
#endif


#ifdef ZOAL_COVERAGE

#define ZOAL_INLINE_IO __attribute__ ((noinline))
#define ZOAL_INLINE_MF __attribute__ ((noinline))

#else

#if ZOAL_INLINE_LEVEL > 0
#define ZOAL_INLINE_IO __attribute__((always_inline)) inline
#else
#define ZOAL_INLINE_IO
#endif

#if ZOAL_INLINE_LEVEL > 1
#define ZOAL_INLINE_MF __attribute__((always_inline)) inline
#else
#define ZOAL_INLINE_MF
#endif

#endif
