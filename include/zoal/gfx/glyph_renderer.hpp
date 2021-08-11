#ifndef ZOAL_GLYPH_RENDERER_HPP
#define ZOAL_GLYPH_RENDERER_HPP

#include "../text/types.hpp"
#include "bitmap_renderer.hpp"

namespace zoal { namespace gfx {
    template<class Graphics, class MemReader>
    class glyph_renderer {
    public:
        using self_type = glyph_renderer<Graphics, MemReader>;
        using pixel_type = typename Graphics::pixel_type;
        using bmp_renderer_type = bitmap_renderer<Graphics, MemReader>;

        glyph_renderer(Graphics *g, const zoal::text::font *font)
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

            x_ += get_kerning(prev_char_code_, ch);
            prev_char_code_ = ch;

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
            prev_char_code_ = 0;
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
            auto glyph_bitmap = font_->bitmap + g->bitmap_offset;
            bmp_renderer_type::draw(
                //
                graphics_,
                glyph_bitmap,
                x_ + g->x_offset,
                y_ + g->y_offset,
                g->width,
                g->height,
                color_);
            x_ += g->x_advance;
        }

        Graphics *graphics_;
        const zoal::text::font *font_{nullptr};
        int x_{0};
        int y_{0};
        uint16_t prev_char_code_{0};
        pixel_type color_;
    };
}}

#endif
