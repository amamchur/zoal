#ifndef ZOAL_I2C_DEVICE_HPP
#define ZOAL_I2C_DEVICE_HPP

#include "./i2c_request.hpp"

namespace zoal { namespace periph {
    enum class device_buffer_type { static_mem, shared_mem };

    template<size_t MinCmdSize, size_t MinDataSize>
    class i2c_device_buffer {
    public:
        static auto constexpr min_cmd_size = MinCmdSize;
        static auto constexpr min_data_size = MinDataSize;

        size_t total_size;
        uint8_t command[min_cmd_size];
        uint8_t data[min_data_size];
    };

    template<class ApiClass>
    class i2c_buffer_owner : public ApiClass {
    public:
        explicit i2c_buffer_owner()
            : ApiClass() {
            this->buffer = &buffer_own;
        }

        explicit i2c_buffer_owner(uint8_t addr)
            : ApiClass(addr) {
            this->buffer = &buffer_own;
        }

    private:
        typename ApiClass::buffer_type buffer_own{0};
    };

    template<class ApiClass>
    class i2c_buffer_guardian : public ApiClass {
    public:
        template<size_t Size>
        i2c_buffer_guardian(void *mem, uint8_t addr)
            : ApiClass(addr) {
            assign_buffer<Size>(mem);
        }

        template<size_t Size>
        explicit i2c_buffer_guardian(void *mem)
            : ApiClass() {
            assign_buffer<Size>(mem);
        }

        template<class T, size_t Size>
        i2c_buffer_guardian(T (&array)[Size], uint8_t addr)
            : ApiClass(addr) {
            assign_buffer<Size * sizeof(T)>(array);
        }

        template<class T, size_t Size>
        explicit i2c_buffer_guardian(T (&array)[Size])
            : ApiClass() {
            assign_buffer<Size * sizeof(T)>(array);
        }

    private:
        template<size_t Size>
        void assign_buffer(void *mem) {
            static_assert(Size >= sizeof(typename ApiClass::buffer_type), "Buffer too small");
            this->buffer = reinterpret_cast<typename ApiClass::buffer_type *>(mem);
            this->buffer->total_size = Size;
        }
    };

    class i2c_device {
    protected:
        template<class Dispatcher, class Callback>
        inline static void next_sequence(Dispatcher &dispatcher, Callback callback) {
            dispatcher.driver_periph_callback.assign(callback);
        }

        template<class Dispatcher>
        inline static void notify_client(Dispatcher &dispatcher, zoal::periph::i2c_request_status status) {
            dispatcher.finish_sequence(status == zoal::periph::i2c_request_status::finished ? 0 : -1);
        };
    };
}}

#endif
