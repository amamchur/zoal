#ifndef ZOAL_BITMAP_RENDERER_HPP
#define ZOAL_BITMAP_RENDERER_HPP

namespace zoal { namespace gfx {
    template<class Graphics, class MemReader>
    class bitmap_renderer {
    public:
        using pixel_type = typename Graphics::pixel_type;

        static void draw(Graphics *g, const uint8_t *bmp, int x, int y, int w, int h, pixel_type c) {
            auto bytes_per_row = (w + 7) >> 3;
            for (int j = 0; j < h; j++) {
                auto row = bmp + j * bytes_per_row;
                for (int i = 0; i < w; i++) {
                    auto ptr = row + (i >> 3);
                    auto mask = 0x80 >> (i & 7);
                    auto byte = MemReader::template read_mem<uint8_t>(ptr);
                    auto value = byte & mask;
                    if (value) {
                        g->pixel(i + x, j + y, c);
                    }
                }
            }
        }
    };
}}

#endif
