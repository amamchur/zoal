#ifndef ZOAL_I2C_MOCK_HPP
#define ZOAL_I2C_MOCK_HPP

#include <zoal/periph/i2c_request.hpp>

namespace zoal { namespace tests {
    template<class T>
    class i2c_mock {
    public:
        static void start(zoal::periph::i2c_request &) {}
    };
}}

#endif
