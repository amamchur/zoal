#ifndef MCURDK_UNOLCDSHIELDAPP_H
#define MCURDK_UNOLCDSHIELDAPP_H

#pragma once

#include <stdint.h>
#include <zoal/gpio/pin_mode.hpp>
#include <zoal/shield/uno_lcd.hpp>
#include <zoal/io/output_stream.hpp>
#include <zoal/utils/method_invoker.hpp>

template<class Tools, class PCB, class Adc>
class uno_lcd_shield {
public:
    using self_type = uno_lcd_shield<Tools, PCB, Adc>;
    using shield = zoal::shield::uno_lcd<Tools, PCB, Adc>;
    using lcd_stream = zoal::io::output_stream;
    using delay = typename Tools::delay;
    using logger = typename Tools::logger;

    typedef struct menu_item {
        const char *text;
        menu_item *next;
        menu_item *prev;
    } menu_item;

    void init() {
        shield::init();
        shield::calibrate(false);

        menuItem0.next = &menuItem1;
        menuItem1.next = &menuItem2;
        menuItem2.next = &menuItem3;
        menuItem3.next = &menuItem4;
        menuItem4.next = &menuItem0;

        menuItem0.prev = &menuItem4;
        menuItem1.prev = &menuItem0;
        menuItem2.prev = &menuItem1;
        menuItem3.prev = &menuItem2;
        menuItem4.prev = &menuItem3;

        menuItem0.text = "Menu Item 0";
        menuItem1.text = "Menu Item 1";
        menuItem2.text = "Menu Item 2";
        menuItem3.text = "Menu Item 3";
        menuItem4.text = "Menu Item 4";

        display_menu_item();
    }

    void display_menu_item() {
        using namespace zoal::io;
        shield::lcd::move(0, 0);
        stream << currentMenuItem->text;
    }

    void button_handler(size_t button, zoal::io::button_event e) {
        using namespace zoal::io;

        if (e != button_event::press) {
            return;
        }

        logger::info() << "buttonHandler: " << button;

        switch (button) {
            case 2:
                currentMenuItem = currentMenuItem->next;
                break;
            case 3:
                currentMenuItem = currentMenuItem->prev;
                break;
            default:
                return;
        }

        shield::lcd::move(0, 0);
        stream << button;
        display_menu_item();
    }

    void run_once() {
        using namespace zoal::io;
        using namespace zoal::utils;

        shield::handle_keypad(method_invoker<self_type, size_t, button_event>(this, &self_type::button_handler));
    }

    menu_item menuItem0;
    menu_item menuItem1;
    menu_item menuItem2;
    menu_item menuItem3;
    menu_item menuItem4;
    menu_item *currentMenuItem{&menuItem0};
    lcd_stream stream{zoal::io::transport_proxy<typename shield::lcd>::instance()};
private:
};

#endif
