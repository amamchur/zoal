#ifndef ZOAL_GFX_GLYPH_RENDER_HPP
#define ZOAL_GFX_GLYPH_RENDER_HPP

namespace zoal { namespace gfx {
    template<class Graphics, class Font, int CharSpacing = 1>
    class glyph_render {
    public:
        using self_type = glyph_render<Graphics, Font, CharSpacing>;
        using pixel_type = typename Graphics::pixel_type;

        explicit glyph_render(Graphics *graphics, const Font *font)
                : graphics_(graphics), font_(font) {}

        void draw(const char *text, pixel_type fg) {
            while (*text) {
                draw(*text++, fg);
            }
        }

        void draw(char c, pixel_type fg) {
            for (size_t i = 0; i < Font::width; i++) {
                auto line = font_->glyph_line(c, i);
                for (size_t j = 0; line != 0; j++) {
                    if (line & 0x1) {
                        graphics_->pixel(x_ + i, y_ + j, fg);
                    }

                    line >>= 1;
                }
            }

            x_ += Font::width + CharSpacing;
        }

        self_type &position(int x, int y) {
            x_ = x;
            y_ = y;
            return *this;
        }

        Graphics *graphics_;
        const Font *font_;
        int x_{0};
        int y_{0};
    };
}}

#endif
