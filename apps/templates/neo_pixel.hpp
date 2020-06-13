#ifndef APPS_NEOPIXELAPP_HPP_
#define APPS_NEOPIXELAPP_HPP_

#include <string.h>
#include <zoal/ic/ws2812.hpp>

template<class Tools, class DI>
class neo_pixel {
public:
    using delay = typename Tools::delay;
    using transmitter = zoal::ic::transmitter_ws2812<DI, Tools::mcu::frequency>;
    using ws2812 = zoal::ic::ws2812<transmitter, delay>;

    typedef struct pixel {
        uint8_t g;
        uint8_t r;
        uint8_t b;
    } pixel;

    pixel pixels[12];
    uint8_t index;
    uint8_t color = 0;

    neo_pixel() : index(0) {
    }

    void init() {
        ws2812::begin();
    }

    void run_once() {
        display();
    }

    void setColor(int index, uint8_t v) {
        if (index >= 12) {
            index -= 12;
        }

        switch (color) {
            case 0:
                pixels[index].r = v;
                break;
            case 1:
                pixels[index].g = v;
                break;
            case 2:
                pixels[index].b = v;
                break;
            default:
                break;
        }
    }

    void display() {
        memset(pixels, 0, sizeof(pixels));
        setColor(index + 0, 1);
        setColor(index + 1, 2);
        setColor(index + 2, 3);
        setColor(index + 3, 4);
        setColor(index + 4, 5);
        setColor(index + 5, 6);

        ws2812::send(pixels, sizeof(pixels) / sizeof(pixels[0]));

        if (++index >= 12) {
            index -= 12;
        }
    }
};

#endif /* APPS_NEOPIXELAPP_HPP_ */
