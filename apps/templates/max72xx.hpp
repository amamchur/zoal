#pragma once

#ifndef APPS_MAX72XX_APP_HPP
#define APPS_MAX72XX_APP_HPP

#include <zoal/gpio/pin_mode.hpp>
#include <zoal/periph/software_spi.hpp>
#include <zoal/ic/max72xx.hpp>

const uint64_t images[] = {
        0x0033333f33331e0c,
        0x003f66663e66663f,
        0x003c66030303663c,
        0x001f36666666361f,
        0x007f46161e16467f,
        0x000f06161e16467f,
        0x007c66730303663c,
        0x003333333f333333,
        0x001e0c0c0c0c0c1e,
        0x001e333330303078,
        0x006766361e366667,
        0x007f66460606060f,
        0x0063636b7f7f7763,
        0x006363737b6f6763,
        0x001c36636363361c,
        0x000f06063e66663f,
        0x00381e3b3333331e,
        0x006766363e66663f,
        0x001e33380e07331e,
        0x001e0c0c0c0c2d3f,
        0x003f333333333333,
        0x000c1e3333333333,
        0x0063777f6b636363,
        0x0063361c1c366363,
        0x001e0c0c1e333333,
        0x007f664c1831637f,
        0x0000000000000000,
        0x006e333e301e0000,
        0x003b66663e060607,
        0x001e3303331e0000,
        0x006e33333e303038,
        0x001e033f331e0000,
        0x000f06060f06361c,
        0x1f303e33336e0000,
        0x006766666e360607,
        0x001e0c0c0c0e000c,
        0x1e33333030300030,
        0x0067361e36660607,
        0x001e0c0c0c0c0c0e,
        0x00636b7f7f330000,
        0x00333333331f0000,
        0x001e3333331e0000,
        0x0f063e66663b0000,
        0x78303e33336e0000,
        0x000f06666e3b0000,
        0x001f301e033e0000,
        0x00182c0c0c3e0c08,
        0x006e333333330000,
        0x000c1e3333330000,
        0x00367f7f6b630000,
        0x0063361c36630000,
        0x1f303e3333330000,
        0x003f260c193f0000
};

const int image_count = sizeof(images) / sizeof(images[0]);

struct font_type {
    union {
        uint64_t glyph[image_count];
        uint8_t glyphRows[image_count][8];
    };
};

template<class Tools, class MOSI, class SCLK, class CS>
class max72xx {
private:
public:
    static constexpr uint8_t DeviceCount = 1;

    using self_type = max72xx<Tools, MOSI, SCLK, CS>;
    using sspi = zoal::gpio::tx_software_spi<MOSI, SCLK>;
    using max7219 = zoal::ic::max72xx<typename Tools::mcu::spi0, CS>;
    using repeat = typename max7219::repeat;
    using method_scheduler = typename Tools::template method_scheduler<self_type, 8>;
    using matrix_type = zoal::ic::max72xx_data<DeviceCount>;

    matrix_type matrix;

    method_scheduler timeout;
    uint8_t shift{0};
    uint8_t intensity{0};
    uint8_t offset{0};
    font_type *font{(font_type *) &images[0]};

    int glyph_index{0};
    int colunm_index{7};

    max72xx() : timeout(this) {
    }

    template<class T>
    static T reverse(T value, uint8_t, uint8_t) {
        T result = 0;
        int bits = sizeof(value) * 8;
        for (int i = 0; i < bits; i++) {
            result <<= 1;
            result |= value & 1;
            value >>= 1;
        }
        return result;
    }

    void init() {
        auto *ptr = (uint8_t *) (&images[0]);
        const int size = sizeof(images);
        for (int i = 0; i < size; i++) {
            ptr[i] = reverse(ptr[i], 0, 0);
        }

        max7219::init(DeviceCount);
        max7219::send(DeviceCount, max7219::intensity | intensity);
        matrix.clear(0);
        max7219::display(matrix);
    }

    void change_intensity() {
        intensity++;
        intensity &= 0xF;
        max7219::transaction() << (max7219::Intencity | intensity);
        timeout.schedule(100, &self_type::change_intensity);
    }

    void push_value() {
        matrix.push_column_lsb(shift);
        shift++;

        max7219::display(matrix);
        timeout.schedule(50, &self_type::push_value);
    }

    void displayAlpha() {
        matrix.data64[0] = images[offset + 3];
        matrix.data64[1] = images[offset + 2];
        matrix.data64[2] = images[offset + 1];
        matrix.data64[3] = images[offset + 0];

        max7219::display(matrix);

        offset += 1;
        if (offset + 4 > image_count) {
            offset = 0;
        }

        timeout.schedule(500, &self_type::displayAlpha);
    }

    void initGlyphColumnMSB() {
        colunm_index = 0;
        glyph_index = 0;
        timeout.schedule(0, &self_type::displayGlyphColumnMSB);
    }

    void displayGlyphColumnMSB() {
        uint8_t column = 0;

        for (int i = 7; i >= 0; i--) {
            column <<= 1;
            column |= (font->glyphRows[glyph_index][i] >> colunm_index) & 1;
        }

        matrix.push_column_msb(column);
        max7219::display(matrix);

        colunm_index++;
        if (colunm_index > 8) {
            colunm_index = 0;
            glyph_index++;
        }

        if (glyph_index >= image_count) {
            glyph_index = 0;
            colunm_index = 0;
        }
        timeout.schedule(50, &self_type::displayGlyphColumnMSB);
    }

    void init_glyph_column_lsb() {
        colunm_index = 7;
        glyph_index = 0;
        timeout.schedule(0, &self_type::displayGlyphColumnLSB);
    }

    void displayGlyphColumnLSB() {
        uint8_t column = 0;

        for (int i = 7; i >= 0; i--) {
            column <<= 1;
            column |= (font->glyphRows[glyph_index][i] >> colunm_index) & 1;
        }

        matrix.insert_row(0, column);
        max7219::display(matrix);

        colunm_index--;
        if (colunm_index < 0) {
            colunm_index = 7;
            glyph_index++;
        }

        if (glyph_index >= image_count) {
            glyph_index = 0;
            colunm_index = 7;
        }
        timeout.schedule(50, &self_type::displayGlyphColumnLSB);
    }

    void run_once() {
        timeout.handle();
    }

    void run() {
        init_glyph_column_lsb();
        while (true) {
            run_once();
        }
    }
};

#endif
