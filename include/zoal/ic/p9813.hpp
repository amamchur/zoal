#ifndef ZOAL_MISC_IC_P9813_HPP
#define ZOAL_MISC_IC_P9813_HPP

namespace zoal { namespace ic {
    template<class SPInterface>
    class p9813 {
    public:
        using spi = SPInterface;
        using self_type = p9813<SPInterface>;

        static void frame() {
            for (auto i = 0; i < 4; i++) {
                spi::transfer_byte(0x00);
            }
        }

        static void send(uint8_t red, uint8_t green, uint8_t blue) {
            uint8_t prefix = 0x00;
            prefix |= (blue & 0xC0) >> 2;
            prefix |= (green & 0xC0) >> 4;
            prefix |= (red & 0xC0) >> 6;

            spi::transfer_byte(~prefix);
            spi::transfer_byte(blue);
            spi::transfer_byte(green);
            spi::transfer_byte(red);
        }
    };
}}

#endif
