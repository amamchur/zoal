#ifndef ZOAL_PROXY_MEM_READER_HPP
#define ZOAL_PROXY_MEM_READER_HPP

namespace zoal { namespace utils {
    class proxy_mem_reader {
    public:
        template<class T>
        static const T& read_mem(const void *ptr) {
            return *reinterpret_cast<const T*>(ptr);
        }
    };
}}

#endif
