#ifndef ZOAL_IC_W25QXX_HPP
#define ZOAL_IC_W25QXX_HPP

namespace zoal { namespace ic {
    template<class Spi, class ChipSelectPin, class Delay>
    class w25qxx {
    private:
        static constexpr uint8_t sr1_busy = 1 << 0;
        static constexpr uint8_t sr1_write_enabled = 1 << 1;
        static constexpr uint8_t sr1_block_protect = 3 << 2;
        static constexpr uint8_t sr1_top_bottom_protect = 1 << 5;
        static constexpr uint8_t sr1_sector_protect = 1 << 6;
        static constexpr uint8_t sr1_status_reg_protect = 1 << 7;

        static void write_enable() {
            typename ws25qxx_cs::_0();
            spi::transfer_byte(0x06);
            typename ws25qxx_cs::_1();
        }

        static void write_disable() {
            typename ws25qxx_cs::_0();
            spi::transfer_byte(0x04);
            typename ws25qxx_cs::_1();
            delay::ms(1);
        }

        static void wait_for_ready() {
            typename ws25qxx_cs::_0();
            spi::transfer_byte(0x05);
            uint8_t sr = spi::transfer_byte(0x00);
            while ((sr & sr1_busy) == sr1_busy) {
                delay::ms(1);
                sr = spi::transfer_byte(0x00);
            }
            typename ws25qxx_cs::_1();
        }

    public:
        using ws25qxx_cs = ChipSelectPin;
        using spi = Spi;
        using delay = Delay;

        static constexpr uint32_t page_size = 0x100;
        static constexpr uint32_t sector_size = 0x1000;
        static constexpr uint32_t block_size = 0x10000;

        static void sector_erase(uint32_t sector) {
            auto address = sector << 12;
            uint8_t cmd[4] = {//
                              0x20,
                              static_cast<uint8_t>((address >> 16) & 0xFF),
                              static_cast<uint8_t>((address >> 8) & 0xFF),
                              static_cast<uint8_t>(address & 0xFF)};
            wait_for_ready();
            write_enable();
            typename ws25qxx_cs::_0();
            spi::transfer(cmd, cmd, sizeof(cmd));
            typename ws25qxx_cs::_1();
            wait_for_ready();
            write_disable();
        }

        static void block_erase(uint32_t block, bool erase64 = false) {
            auto address = block << 16;
            uint8_t cmd[4] = {//
                              static_cast<uint8_t>(erase64 ? 0xD8 : 0x20),
                              static_cast<uint8_t>((address >> 16) & 0xFF),
                              static_cast<uint8_t>((address >> 8) & 0xFF),
                              static_cast<uint8_t>(address & 0xFF)};
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
            wait_for_ready();
            write_enable();
            wait_for_ready();
            typename ws25qxx_cs::_0();
            spi::transfer_byte(0xC7);
            typename ws25qxx_cs::_1();
            wait_for_ready();
            write_disable();
            wait_for_ready();
        }

        static void page_program(uint32_t addr, const void *buffer, size_t size) {
            uint8_t cmd[4] = {//
                              0x02,
                              static_cast<uint8_t>((addr >> 16) & 0xFF),
                              static_cast<uint8_t>((addr >> 8) & 0xFF),
                              static_cast<uint8_t>(addr & 0xFF)};
            write_enable();
            wait_for_ready();

            typename ws25qxx_cs::_0();
            spi::transfer(cmd, cmd, sizeof(cmd));
            spi::transfer(buffer, nullptr, size);
            typename ws25qxx_cs::_1();

            wait_for_ready();
            write_disable();
        }

        static void write(uint32_t addr, const void *buffer, size_t size) {
            auto ptr = reinterpret_cast<const uint8_t *>(buffer);
            while (size > 0) {
                auto length = 0x100 - (0xFF & addr);
                if (length > size) {
                    length = size;
                }

                page_program(addr, ptr, length);
                size -= length;
                addr += length;
                ptr += length;
            }
        }

        static void fast_read(uint32_t addr, void *buffer, size_t size) {
            uint8_t cmd[5] = {//
                              0x0B,
                              static_cast<uint8_t>((addr >> 16) & 0xFF),
                              static_cast<uint8_t>((addr >> 8) & 0xFF),
                              static_cast<uint8_t>(addr & 0xFF),
                              0};
            wait_for_ready();
            typename ws25qxx_cs::_0();
            spi::transfer(cmd, cmd, sizeof(cmd));
            spi::transfer(buffer, buffer, size);
            typename ws25qxx_cs::_1();
        }

        static void read(uint32_t addr, void *buffer, size_t size) {
            auto ptr = reinterpret_cast<uint8_t *>(buffer);
            while (size > 0) {
                auto length = 0x100 - (0xFF & addr);
                if (length > size) {
                    length = size;
                }

                fast_read(addr, ptr, length);
                size -= length;
                addr += length;
                ptr += length;
            }
        }

        inline static uint32_t address_to_sector(uint32_t addr) {
            return addr >> 12;
        }

        inline static uint32_t address_to_block(uint32_t addr) {
            return addr >> 16;
        }
    };
}}

#endif
