#ifndef ZOAL_INLINE_LEVEL
#define ZOAL_INLINE_LEVEL 4
#endif

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
