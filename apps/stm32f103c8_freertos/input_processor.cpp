#include "input_processor.hpp"

#include "command_queue.hpp"
#include "hardware.hpp"
#include "task.h"

#include <zoal/io/rotary_encoder.hpp>

//using enc_type = zoal::io::rotary_encoder<mcu::pa_11, mcu::pa_12, rotary_2phase_machine_v2, zoal::gpio::pin_mode::input>;
using enc_type = zoal::io::rotary_encoder<mcu::pa_11, mcu::pa_12, zoal::io::rotary_2phase_machine_v2>;
enc_type encoder;

//static user_button_1_type user_button_1;
//static user_button_2_type user_button_2;
//auto button_callback = [](zoal::io::button_event e, app_cmd cmd) {
//    if (e == zoal::io::button_event::press) {
//        command_queue.push(command_msg(cmd));
//    }
//};

[[noreturn]] void zoal_input_processor(void *) {
    using namespace zoal::gpio;


    api::optimize<api::mode<pin_mode::input, mcu::pa_11, mcu::pa_12>>();

    vTaskDelay(100);

    while (true) {
        vTaskDelay(1);
        encoder.handle([](zoal::io::rotary_event e) {
            scoped_lock lock(tx_stream_mutex);
            tx_stream << (int)e << "\r\n";
        });

        //        auto time = xTaskGetTickCount();
        //        user_button_1.handle(time, button_callback, app_cmd::button1_pressed);
        //        user_button_2.handle(time, button_callback, app_cmd::button2_pressed);
    }
}
