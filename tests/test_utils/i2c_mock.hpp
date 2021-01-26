#ifndef ZOAL_I2C_MOCK_HPP
#define ZOAL_I2C_MOCK_HPP
namespace zoal { namespace tests {
    template<class T>
    class i2c_mock {
    public:
        static void start() {}
    };
}}

#endif
