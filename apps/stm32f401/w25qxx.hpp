#ifndef ZOAL_IC_W25QXX_HPP
#define ZOAL_IC_W25QXX_HPP

namespace zoal { namespace ic {
    template<class Spi, class ChipSelectPin, class Delay>
    class w25qxx {
    public:
        using ws25qxx_cs = ChipSelectPin;
        using spi = Spi;
        using delay = Delay;

        static void init() {}

        static void erase_sector(uint32_t addr) {
            uint8_t cmd[6] = {//
                              0x20,
                              static_cast<uint8_t>((addr >> 16) & 0xFF),
                              static_cast<uint8_t>((addr >> 8) & 0xFF),
                              static_cast<uint8_t>(addr & 0xFF),
                              0,
                              0};
            ws25qxx_cs::_0();
            wait_for_ready();
            write_enable();
            delay::ms(1);
            spi::transfer(cmd, cmd, sizeof(cmd));
            delay::ms(1);
            wait_for_ready();
            write_disable();
            ws25qxx_cs::_1();
        }

        static void write_data(uint32_t addr) {}

        static void read_data(uint32_t addr, void *buffer, size_t size) {
            uint8_t cmd[6] = {//
                              0x0B,
                              static_cast<uint8_t>((addr >> 16) & 0xFF),
                              static_cast<uint8_t>((addr >> 8) & 0xFF),
                              static_cast<uint8_t>(addr & 0xFF),
                              0,
                              0};
            uint8_t rx[16];
            cmd[0] = 0x0B;
            cmd[1] = (addr >> 16) & 0xFF;
            cmd[2] = (addr >> 8) & 0xFF;
            cmd[3] = addr & 0xFF;
            cmd[4] = 0;
            cmd[5] = 0;
            typename ws25qxx_cs::_0();
            spi::transfer(cmd, cmd, sizeof(cmd));
            spi::transfer(buffer, buffer, size);
            typename ws25qxx_cs::_1();
        }

    private:
        static void write_enable() {
            uint8_t cmd[] = {0x06};
            spi::transfer(cmd, cmd, sizeof(cmd));
            delay::ms(1);
        }

        static void write_disable() {
            uint8_t cmd[] = {0x04};
            ws25qxx_cs::_0();
            spi::transfer(cmd, cmd, sizeof(cmd));
            ws25qxx_cs::_1();
            delay::ms(1);
        }

        static void wait_for_ready() {
            uint8_t sr;
            spi::transfer_byte(0x05);
            do {
                sr = spi::transfer_byte(0x00);
                delay::ms(1);
            } while ((sr & 0x01) == 0x01);
        }
    };
}}

#endif
