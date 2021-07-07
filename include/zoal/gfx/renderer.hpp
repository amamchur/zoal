#ifndef ZOAL_GFX_RENDERER_HPP
#define ZOAL_GFX_RENDERER_HPP

#include <stdlib.h>

namespace zoal { namespace gfx {
    template<class Pixel, class Adapter>
    class renderer {
    public:
        using self_type = renderer<Pixel, Adapter>;
        using pixel_type = Pixel;
        using adapter_type = Adapter;

        static self_type *from_memory(void *buffer) {
            return reinterpret_cast<self_type *>(buffer);
        }

        inline void clear(pixel_type v) {
            adapter_type::clear(buffer, v);
        }

        inline void pixel(int x, int y, pixel_type c) {
            adapter_type::pixel(buffer, x, y, c);
        }

        void swap(pixel_type &a, pixel_type &b) {
            auto tmp = a;
            a = b;
            b = tmp;
        }

        void draw_line(int x0, int y0, int x1, int y1, pixel_type color) {
            int dx = abs(x1 - x0);
            int sx = x0 < x1 ? 1 : -1;
            int dy = abs(y1 - y0);
            int sy = y0 < y1 ? 1 : -1;
            int error = (dx > dy ? dx : -dy) / 2;

            for (;;) {
                pixel(x0, y0, color);
                if (x0 == x1 && y0 == y1) {
                    break;
                }

                int e = error;
                if (e > -dx) {
                    error -= dy;
                    x0 += sx;
                }

                if (e < dy) {
                    error += dx;
                    y0 += sy;
                }
            }
        }

        void draw_rect(int x, int y, int w, int h, pixel_type color) {
            draw_line(x, y, x + w, y, color);
            draw_line(x, y + h, x + w, y + h, color);
            draw_line(x, y, x, y + h, color);
            draw_line(x + w, y, x + w, y + h, color);
        }

        void fill_rect(int x, int y, int w, int h, Pixel color) {
            for (int i = x; i <= x + w; i++) {
                draw_line(i, y, i, y + h, color);
            }
        }

        template<class Horizontal, class Vertical>
        void render_circle(int xc, int yc, int r, pixel_type color, Horizontal h, Vertical v) {
            int f = 1 - r;
            int ddf_x = 1;
            int ddf_y = -2 * r;
            int x = 0;
            int y = r;

            v(xc, yc, r, color);
            h(xc, yc, r, color);

            while (x < y) {
                if (f >= 0) {
                    y--;
                    ddf_y += 2;
                    f += ddf_y;
                }
                x++;
                ddf_x += 2;
                f += ddf_x;

                h(xc, yc + y, x, color);
                h(xc, yc - y, x, color);
                h(xc, yc + x, y, color);
                h(xc, yc - x, y, color);
            }
        }

        void draw_circle(int x, int y, int r, pixel_type color) {
            void *ptr = buffer;
            render_circle(x, y, r, color,
                          [ptr](int x, int y, int d, pixel_type color) {
                              adapter_type::pixel(ptr, x + d, y, color);
                              adapter_type::pixel(ptr, x - d, y, color);
                          },
                          [ptr](int x, int y, int d, pixel_type color) {
                              adapter_type::pixel(ptr, x, y + d, color);
                              adapter_type::pixel(ptr, x, y - d, color);
                          });
        }

        void fill_circle(int x, int y, int r, pixel_type color) {
            render_circle(x, y, r, color,
                          [this](int x, int y, int d, pixel_type color) {
                              draw_line(x - d, y, x + d, y, color);
                          },
                          [this](int x, int y, int d, pixel_type color) {
                              draw_line(x, y - d, x, y + d, color);
                          });
        }

    private:
        uint8_t buffer[0];
    };
}}

#endif
