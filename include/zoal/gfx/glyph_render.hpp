#ifndef ZOAL_GLYPH_RENDER_HPP
#define ZOAL_GLYPH_RENDER_HPP

#include "../text/types.hpp"

#include <avr/pgmspace.h>

namespace zoal { namespace gfx {
    template<class Graphics>
    class glyph_render {
    public:
        using self_type = glyph_render<Graphics>;
        using pixel_type = typename Graphics::pixel_type;

        glyph_render(Graphics *g, const zoal::text::font *font)
            : graphics_(g)
            , font_(font) {}

        void draw(const wchar_t ch, pixel_type fg) {
            const zoal::text::unicode_range *range = nullptr;
            auto code = (uint16_t)ch;
            for (size_t i = 0; i < font_->ranges_count; i++) {
                const zoal::text::unicode_range *r = font_->ranges + i;
                if (r->start <= code && code <= r->end) {
                    range = r;
                    break;
                }
            }

            if (range == nullptr) {
                return;
            }

            auto pos = code - range->start + range->base;
            //            auto glyph = font_->glyphs + pos;
            //            render_glyph(glyph);

            zoal::text::glyph g;
            memcpy_P(&g, font_->glyphs + pos, sizeof(g));
            render_glyph(&g, fg);
        }

        void draw(const wchar_t *text, pixel_type fg) {
            while (*text) {
                draw(*text++, fg);
            }
        }

        void draw(const char *text, pixel_type fg) {
            while (*text) {
                draw(*text++, fg);
            }
        }

        self_type &position(int x, int y) {
            x_ = x;
            y_ = y;
            return *this;
        }

    private:
        void render_glyph(const zoal::text::glyph *g, pixel_type fg) {
            const uint8_t *data = font_->bitmap + g->bitmap_offset;
            const uint8_t bytes_per_row = (g->width + 7) >> 3;
            for (int y = 0; y < g->height; y++) {
                auto row = data + y * bytes_per_row;
                for (int x = 0; x < g->width; x++) {
                    auto ptr = row + (x >> 3);
                    int mask = 0x80 >> (x & 7);
                    uint8_t qqq = pgm_read_byte(ptr);
                    int value = qqq & mask;
                    if (value) {
                        graphics_->pixel(x_ + x + g->x_offset, y_ + y + g->y_offset, fg);
                    }
                }
            }
            x_ += g->x_advance;
        }

        Graphics *graphics_;
        const zoal::text::font *font_{nullptr};
        int x_{0};
        int y_{0};
    };
}}

#endif
