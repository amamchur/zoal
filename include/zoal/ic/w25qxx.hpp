#ifndef ZOAL_IC_W25QXX_HPP
#define ZOAL_IC_W25QXX_HPP

namespace zoal { namespace ic {
    template<class Spi, class ChipSelectPin, class Delay>
    class w25qxx {
    public:
        using ws25qxx_cs = ChipSelectPin;
        using spi = Spi;
        using delay = Delay;

        static void sector_sector(uint32_t addr) {
            uint8_t cmd[4] = {//
                              0x20,
                              static_cast<uint8_t>((addr >> 16) & 0xFF),
                              static_cast<uint8_t>((addr >> 8) & 0xFF),
                              static_cast<uint8_t>(addr & 0xFF)};
            wait_for_ready();
            write_enable();
            typename ws25qxx_cs::_0();
            spi::transfer(cmd, cmd, sizeof(cmd));
            typename ws25qxx_cs::_1();
            wait_for_ready();
            write_disable();
            wait_for_ready();
        }

        static void block_erase(uint32_t addr, bool erase64 = false) {
            uint8_t cmd[4] = {//
                              static_cast<uint8_t>(erase64 ? 0xD8 : 0x20),
                              static_cast<uint8_t>((addr >> 16) & 0xFF),
                              static_cast<uint8_t>((addr >> 8) & 0xFF),
                              static_cast<uint8_t>(addr & 0xFF)};
            wait_for_ready();
            write_enable();
            typename ws25qxx_cs::_0();
            spi::transfer(cmd, cmd, sizeof(cmd));
            typename ws25qxx_cs::_1();
            wait_for_ready();
            write_disable();
            wait_for_ready();
        }

        static void chip_erase() {
            uint8_t cmd[1] = {0xC7};
            wait_for_ready();
            write_enable();
            wait_for_ready();
            typename ws25qxx_cs::_0();
            spi::transfer(cmd, cmd, sizeof(cmd));
            typename ws25qxx_cs::_1();
            wait_for_ready();
            write_disable();
            wait_for_ready();
        }

        static void page_program(uint32_t addr, void *buffer, size_t size) {
            uint8_t cmd[4] = {//
                              0x02,
                              static_cast<uint8_t>((addr >> 16) & 0xFF),
                              static_cast<uint8_t>((addr >> 8) & 0xFF),
                              static_cast<uint8_t>(addr & 0xFF)};
            write_enable();
            wait_for_ready();

            typename ws25qxx_cs::_0();
            spi::transfer(cmd, cmd, sizeof(cmd));
            spi::transfer(buffer, buffer, size);
            typename ws25qxx_cs::_1();

            write_disable();
            wait_for_ready();
        }

        static void fast_read(uint32_t addr, void *buffer, size_t size) {
            uint8_t cmd[5] = {//
                              0x0B,
                              static_cast<uint8_t>((addr >> 16) & 0xFF),
                              static_cast<uint8_t>((addr >> 8) & 0xFF),
                              static_cast<uint8_t>(addr & 0xFF),
                              0};
            typename ws25qxx_cs::_0();
            spi::transfer(cmd, cmd, sizeof(cmd));
            spi::transfer(buffer, buffer, size);
            typename ws25qxx_cs::_1();
        }

    private:
        static void write_enable() {
            uint8_t cmd[] = {0x06};
            typename ws25qxx_cs::_0();
            spi::transfer(cmd, cmd, sizeof(cmd));
            typename ws25qxx_cs::_1();
        }

        static void write_disable() {
            uint8_t cmd[] = {0x04};
            typename ws25qxx_cs::_0();
            spi::transfer(cmd, cmd, sizeof(cmd));
            typename ws25qxx_cs::_1();
            delay::ms(1);
        }

        static void wait_for_ready() {
            uint8_t sr;
            typename ws25qxx_cs::_0();
            spi::transfer_byte(0x05);
            do {
                sr = spi::transfer_byte(0x00);
                delay::ms(1);
            } while ((sr & 0x01) == 0x01);
            typename ws25qxx_cs::_1();
        }
    };
}}

#endif
