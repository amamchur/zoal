#ifndef ZOAL_FREERTOS_SCOPED_LOCK_HPP
#define ZOAL_FREERTOS_SCOPED_LOCK_HPP

namespace zoal { namespace freertos {
    template<class Mutex>
    class scoped_lock {
    public:
        inline explicit scoped_lock(Mutex &m) noexcept
            : mutex(m) {
            mutex.lock();
        }

        scoped_lock(const scoped_lock&) = delete;

        inline ~scoped_lock() noexcept {
            mutex.unlock();
        }

    private:
        Mutex &mutex;
    };
}}

#endif
