#ifndef ZOAL_I2C_DEVICE_HPP
#define ZOAL_I2C_DEVICE_HPP

namespace zoal { namespace periph {
    class i2c_device {
    protected:
        template<class M, class S, class F>
        void next_sequence(M &m, S s, F f) {
            m.success_periph_callback.assign(s);
            m.failed_periph_callback.assign(f);
        }

        template<class M, class S>
        void next_sequence(M &m, S s) {
            m.success_periph_callback.assign(s);
            m.failed_periph_callback.reset();
        }
    };
}}

#endif
