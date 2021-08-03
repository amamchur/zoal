#ifndef ZOAL_GLYPH_RENDER_HPP
#define ZOAL_GLYPH_RENDER_HPP

#include "../text/types.hpp"

namespace zoal { namespace gfx {
    template<class Graphics, class MemReader>
    class glyph_render {
    public:
        using self_type = glyph_render<Graphics, MemReader>;
        using pixel_type = typename Graphics::pixel_type;

        glyph_render(Graphics *g, const zoal::text::font *font)
            : graphics_(g)
            , font_(font) {}

        int8_t get_kerning(uint16_t f, uint16_t s) {
            if (font_->kerning_pairs_count == 0) {
                return 0;
            }

            auto l = font_->kerning_pairs;
            auto r = l + font_->kerning_pairs_count - 1;
            const zoal::text::kerning_pair *k = nullptr;
            while (l < r) {
                auto m = l + ((r - l) >> 1);
                auto kp = MemReader::template read_mem<zoal::text::kerning_pair>(m);
                if (kp.first == f) {
                    k = m;
                    break;
                }

                if (kp.first < f) {
                    l = m + 1;
                }

                if (kp.first > f) {
                    r = m - 1;
                }
            }

            if (k == nullptr) {
                return 0;
            }

            l = font_->kerning_pairs;
            r = l + font_->kerning_pairs_count - 1;
            for (auto p = k; p >= l; p--) {
                auto kp = MemReader::template read_mem<zoal::text::kerning_pair>(p);
                if (kp.second == s) {
                    return kp.x_advance;
                }
            }

            for (auto p = k; p <= r; p++) {
                auto kp = MemReader::template read_mem<zoal::text::kerning_pair>(p);
                if (kp.second == s) {
                    return kp.x_advance;
                }
            }

            return 0;
        }

        void draw(const wchar_t ch) {
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

            x_ += get_kerning(last_char_code_, ch);
            last_char_code_ = ch;

            auto pos = code - range->start + range->base;
            auto g = MemReader::template read_mem<zoal::text::glyph>(font_->glyphs + pos);
            render_glyph(&g);
        }

        void draw(const wchar_t *text) {
            while (*text) {
                draw(*text++);
            }
        }

        void draw(const char *text) {
            while (*text) {
                draw(*text++);
            }
        }

        void draw(const char *start, const char *end) {
            while (start != end) {
                draw(*start++);
            }
        }

        self_type &position(int x, int y) {
            x_ = x;
            y_ = y;
            last_char_code_ = 0;
            return *this;
        }

        pixel_type color() const {
            return color_;
        }

        void color(pixel_type c) {
            color_ = c;
        }

        void send_byte(uint8_t v) {
            draw(v);
        }

        void send_data(const void *data, size_t size) {
            auto d = reinterpret_cast<const uint8_t *>(data);
            for (size_t i = 0; i < size; i++) {
                draw(d[i]);
            }
        }
    private:
        void render_glyph(const zoal::text::glyph *g) {
            const uint8_t *data = font_->bitmap + g->bitmap_offset;
            const uint8_t bytes_per_row = (g->width + 7) >> 3;
            for (int y = 0; y < g->height; y++) {
                auto row = data + y * bytes_per_row;
                for (int x = 0; x < g->width; x++) {
                    auto ptr = row + (x >> 3);
                    int mask = 0x80 >> (x & 7);
                    auto qqq = MemReader::template read_mem<uint8_t>(ptr);
                    int value = qqq & mask;
                    if (value) {
                        graphics_->pixel(x_ + x + g->x_offset, y_ + y + g->y_offset, color_);
                    }
                }
            }
            x_ += g->x_advance;
        }

        Graphics *graphics_;
        const zoal::text::font *font_{nullptr};
        int x_{0};
        int y_{0};
        uint16_t last_char_code_{0};
        pixel_type color_;
    };
}}

#endif
