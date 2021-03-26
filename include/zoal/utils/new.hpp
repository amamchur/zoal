#ifndef ZOAL_UTILS_NEW_HPP
#define ZOAL_UTILS_NEW_HPP

template<class T>
inline void* operator new(size_t count, T* ptr ) {
    return ptr;
}

#endif
