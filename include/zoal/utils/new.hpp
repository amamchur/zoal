#ifndef ZOAL_UTILS_NEW_HPP
#define ZOAL_UTILS_NEW_HPP

inline void* operator new(size_t count, void* ptr ) {
    return ptr;
}

#endif
