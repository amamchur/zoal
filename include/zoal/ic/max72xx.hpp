#ifndef ZOAL_IC_MAX7221_HPP
#define ZOAL_IC_MAX7221_HPP

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>

#include "../gpio/pin_mode.hpp"

namespace zoal { namespace ic {
    template<size_t Devices>
    class max72xx_data {
    public:
        static constexpr size_t devices = Devices;

        union {
            uint8_t data[Devices][8];
            uint64_t data64[Devices];
        };

        template<class Iter>
        max72xx_data &operator=(Iter iter) {
            uint8_t *ptr = &data[0][0];
            for (int i = 0; i < Devices; i++) {
                for (int j = 0; j < 8; j++) {
                    *ptr++ = iter(i, j);
                }
            }
            return *this;
        }

        max72xx_data &operator=(const max72xx_data &matrix) {
            memcpy(data, matrix.data, sizeof(data));
            return *this;
        }

        template<class Iter>
        max72xx_data &operator|=(Iter iter) {
            uint8_t *ptr = &data[0][0];
            for (int i = 0; i < Devices; i++) {
                for (int j = 0; j < 8; j++) {
                    *ptr++ |= iter(i, j);
                }
            }
            return *this;
        }

        max72xx_data &operator|=(const max72xx_data &matrix) {
            return *this |= &matrix.data[0][0];
        }

        template<class Iter>
        max72xx_data &operator&=(Iter iter) {
            uint8_t *ptr = &data[0][0];
            for (int i = 0; i < Devices; i++) {
                for (int j = 0; j < 8; j++) {
                    *ptr++ &= iter(i, j);
                }
            }
            return *this;
        }

        inline void clear(uint8_t value = 0) {
            memset(data, value, sizeof(data));
        }

        template<class F>
        void transform_row(F f) {
            for (int i = 0; i < Devices; i++) {
                for (int j = 0; j < 8; j++) {
                    data[i][j] = f(data[i][j], i, j, *this);
                }
            }
        }

        template<class F>
        void transform_row(int device, F f) {
            for (int j = 0; j < 8; j++) {
                data[device][j] = f(data[device][j], device, j, *this);
            }
        }

        template<class F>
        void transform_data(F f) {
            for (int i = 0; i < Devices; i++) {
                data64[i] = f(data64[i], i, *this);
            }
        }

        template<typename Formatter>
        void print_int(Formatter formatter, int32_t value) {
            clear(formatter(' '));

            data[0][0] = formatter('0');

            bool negative = false;
            if (value < 0) {
                value = -value;
                negative = true;
            }

            uint8_t device = 0;
            uint8_t row = 0;
            while (value > 0) {
                char v = static_cast<char>(value % 10);
                value = value / 10;
                data[device][row] = formatter(v + 0x30);
                row++;
                if (row > 7) {
                    row = 0;
                    device++;
                    if (device >= Devices) {
                        return;
                    }
                }
            }

            if (negative) {
                data[device][row] = formatter('-');
            }
        }

        template<typename Formatter>
        void print_int(Formatter formatter, uint32_t value) {
            clear(formatter(' '));
            data[0][0] = formatter('0');

            uint8_t device = 0;
            uint8_t row = 0;
            while (value > 0) {
                char v = static_cast<char>(value % 10);
                value = value / 10;
                data[device][row] = formatter(v + 0x30);
                row++;
                if (row > 7) {
                    row = 0;
                    device++;
                    if (device >= Devices) {
                        return;
                    }
                }
            }
        }

        template<typename Formatter>
        void print(Formatter formatter, double value, uint8_t precition = 2) {
            double int_part = 0.0;
            double fract_part = modf(value, &int_part);
            print_int(formatter, (int32_t) int_part);

            if (fract_part < 0) {
                fract_part = -fract_part;
            }

            data[0][0] |= 0x80;

            while (precition > 0) {
                fract_part = modf(fract_part * 10, &int_part);
                push_row(0, static_cast<uint8_t>(int_part));
                precition--;
            }
        }

        template<typename Formatter, typename T>
        void print_hex(Formatter formatter, T value) {
            uint8_t count = sizeof(T) * 2;
            uint8_t device = 0;
            uint8_t row = 0;
            while (count > 0) {
                data[device][row] = formatter(value & 0xF);
                row++;
                if (row > 7) {
                    row = 0;
                    device++;
                    if (device >= Devices) {
                        return;
                    }
                }
                value >>= 4;
                count--;
            }
        }

        uint8_t insert_row(uint8_t device, uint8_t row) {
            uint8_t *ptr = &data[device][0];
            uint8_t r = data[device][7];
            for (int i = 7; i > 0; i--) {
                ptr[i] = ptr[i - 1];
            }

            data[device][0] = row;

            return r;
        }

        uint8_t push_row(uint8_t device, uint8_t row) {
            uint8_t *ptr = &data[device][0];
            uint8_t r = *ptr;
            for (int i = 0; i < 7; i++) {
                ptr[i] = ptr[i + 1];
            }

            data[device][7] = row;

            return r;
        }

        void push_rows(const uint8_t *rows) {
            for (ptrdiff_t i = Devices - 1; i > 0; i--) {
                for (int j = 0; j < 8; j++) {
                    data[i][j] = data[i - 1][j];
                }
            }

            for (int i = 0; i < 8; i++) {
                data[0][i] = rows[i];
            }
        }

        void push_rows(const uint64_t *rows) {
            return push_rows((const uint8_t *) rows);
        }

        void set_rows(int device, const uint8_t *rows) {
            for (int i = 0; i < 8; i++) {
                data[device][i] = rows[i];
            }
        }

        void set_rows(int device, const uint64_t *rows) {
            return set_rows(device, (const uint8_t *) rows);
        }

        void push_column_msb(uint8_t column) {
            for (int j = 0; j < 8; j++) {
                auto carry_flag = static_cast<uint8_t>((column >> j) & 1);
                for (ptrdiff_t i = Devices - 1; i >= 0; i--) {
                    auto tmp = static_cast<uint8_t>(data[i][j] & 1);
                    data[i][j] >>= 1;
                    data[i][j] |= carry_flag << 7;
                    carry_flag = tmp;
                }
            }
        }

        void push_column_lsb(uint8_t column) {
            for (int j = 0; j < 8; j++) {
                auto carry_flag = static_cast<uint8_t>((column >> j) & 1);
                for (auto i = 0; i < Devices; i++) {
                    uint8_t tmp = data[i][j] >> 7;
                    data[i][j] <<= 1;
                    data[i][j] |= carry_flag;
                    carry_flag = tmp;
                }
            }
        }
    };

    template<class SPInterface, class ChipSelect>
    class max72xx {
    public:
        using spi = SPInterface;
        enum Command : uint16_t {
            noop = 0x0000,
            data_row0 = 0x0100,
            data_row1 = 0x0200,
            data_row2 = 0x0300,
            data_row3 = 0x0400,
            data_row4 = 0x0500,
            data_row5 = 0x0600,
            data_row6 = 0x0700,
            data_row7 = 0x0800,
            decode_mode = 0x0900,
            decode_mode_off = 0x0900,
            intensity = 0x0A00,
            intensity0 = 0x0A00,
            intensity1 = 0x0A01,
            intensity2 = 0x0A02,
            intensity3 = 0x0A03,
            intensity4 = 0x0A04,
            intensity5 = 0x0A05,
            intensity6 = 0x0A06,
            intensity7 = 0x0A07,
            intensity8 = 0x0A08,
            intensity9 = 0x0A09,
            intensityA = 0x0A0A,
            intensityB = 0x0A0B,
            intensityC = 0x0A0C,
            intensityD = 0x0A0D,
            intensityE = 0x0A0E,
            intensityF = 0x0A0F,
            scan_limit = 0x0B00,
            scan_limit_off = 0x0B00,
            scan_limit_on = 0x0B01,
            on_off = 0x0C00,
            off = 0x0C00,
            on = 0x0C01,
            test = 0x0F00,
            test_off = 0x0F00,
            test_on = 0x0F01
        };

        class repeat {
        public:
            size_t times;
            uint16_t cmd;

            repeat(size_t times, uint16_t cmd) : times(times), cmd(cmd) {}
        };

        class transaction {
        public:
            transaction() {
                ChipSelect::low();
            }

            transaction &operator<<(uint16_t cmd) {
                spi::transfer(cmd);
                return *this;
            }

            transaction &operator<<(const repeat &n) {
                const auto h = n.cmd >> 0x8;
                const auto l = n.cmd & 0x0F;

                for (uint16_t i = 0; i < n.times; i++) {
                    spi::transfer_byte(h);
                    spi::transfer_byte(l);
                }
                return *this;
            }

            ~transaction() {
                ChipSelect::high();
            }
        };

        max72xx() = delete;

        static void init(uint8_t devices) {
            using namespace zoal::gpio;

            ChipSelect::template mode<pin_mode::output>();
            ChipSelect::high();

            send(devices, test_off);
            send(devices, scan_limit | 0x07);
            send(devices, decode_mode_off);
            send(devices, on);
        }

        template<size_t Devices>
        static void display(const max72xx_data<Devices> &matrix) {
            for (uint8_t row = 0; row < 8; row++) {
                ChipSelect::low();
                auto cmd = row + 1;
                for (int device = Devices - 1; device >= 0; device--) {
                    spi::transfer_byte(cmd);
                    spi::transfer_byte(matrix.data[device][row]);
                }
                ChipSelect::high();
            }
        }

        static void send(size_t times, uint16_t cmd) {
            transaction() << repeat(times, cmd);
        }
    };
}}

#endif
