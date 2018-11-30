#ifndef ZOAL_ALGORITHM_BRESENHAM_GFX_HPP
#define ZOAL_ALGORITHM_BRESENHAM_GFX_HPP

namespace zoal { namespace algorithm {

    template<class Pixel, class Adapter>
    class bresenham_gfx {
    public:
        using self_type = bresenham_gfx<Pixel, Adapter>;
        using pixel_type = Pixel;
        using adapter_type = Adapter;

        static self_type *from_memory(void *buffer) {
            return reinterpret_cast<self_type *>(buffer);
        }

        inline void clear(Pixel v) {
            adapter_type::clear(data, v);
        }

        inline void pixel(int x, int y, Pixel c) {
            adapter_type::pixel(data, x, y, c);
        }

        void swap(Pixel &a, Pixel &b) {
            auto tmp = a;
            a = b;
            b = tmp;
        }

        void draw_line(int x0, int y0, int x1, int y1, Pixel color) {
            int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
            int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
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

        void draw_circle(int x0, int y0, int radius, Pixel color) {
            int x = radius - 1;
            int y = 0;
            int dx = 1;
            int dy = 1;
            int error = dx - (radius << 1);

            while (x >= y) {
                pixel(x0 + x, y0 + y, color);
                pixel(x0 + y, y0 + x, color);
                pixel(x0 - y, y0 + x, color);
                pixel(x0 - x, y0 + y, color);
                pixel(x0 - x, y0 - y, color);
                pixel(x0 - y, y0 - x, color);
                pixel(x0 + y, y0 - x, color);
                pixel(x0 + x, y0 - y, color);

                if (error <= 0) {
                    y++;
                    error += dy;
                    dy += 2;
                }

                if (error > 0) {
                    x--;
                    dx += 2;
                    error += dx - (radius << 1);
                }
            }
        }

        uint8_t data[0];
    };
}}

#endif
