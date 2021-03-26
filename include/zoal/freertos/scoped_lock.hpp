#ifndef ZOAL_FREERTOS_SCOPED_LOCK_HPP
#define ZOAL_FREERTOS_SCOPED_LOCK_HPP

namespace zoal { namespace freertos {
    template<class Mutex>
    class scoped_lock {
    public:
        explicit scoped_lock(Mutex &m) noexcept
            : mutex(m) {
            mutex.lock();
        }

        scoped_lock(const scoped_lock&) = delete;

        ~scoped_lock() noexcept {
            mutex.unlock();
        }

        Mutex &mutex;
    };
}}

#endif
