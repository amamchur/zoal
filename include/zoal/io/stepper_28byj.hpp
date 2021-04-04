#ifndef ZOAL_IO_STEPPER_28BYJ_HPP
#define ZOAL_IO_STEPPER_28BYJ_HPP

#include "../gpio/api.hpp"

namespace zoal { namespace io {
    template<int steps, class pin_a, class pin_b, class pin_c, class pin_d>
    class stepper_28byj_mode {};

    template<class pin_a, class pin_b, class pin_c, class pin_d>
    class stepper_28byj_mode<8, pin_a, pin_b, pin_c, pin_d> {
    public:
        using api = zoal::gpio::api;

        using step0 = api::optimize<
            //
            typename pin_a::_1,
            typename pin_b::_0,
            typename pin_c::_0,
            typename pin_d::_0>;
        using step1 = api::optimize<
            //
            typename pin_a::_1,
            typename pin_b::_1,
            typename pin_c::_0,
            typename pin_d::_0>;
        using step2 = api::optimize<
            //
            typename pin_a::_0,
            typename pin_b::_1,
            typename pin_c::_0,
            typename pin_d::_0>;
        using step3 = api::optimize<
            //
            typename pin_a::_0,
            typename pin_b::_1,
            typename pin_c::_1,
            typename pin_d::_0>;
        using step4 = api::optimize<
            //
            typename pin_a::_0,
            typename pin_b::_0,
            typename pin_c::_1,
            typename pin_d::_0>;
        using step5 = api::optimize<
            //
            typename pin_a::_0,
            typename pin_b::_0,
            typename pin_c::_1,
            typename pin_d::_1>;
        using step6 = api::optimize<
            //
            typename pin_a::_0,
            typename pin_b::_0,
            typename pin_c::_0,
            typename pin_d::_1>;
        using step7 = api::optimize<
            //
            typename pin_a::_1,
            typename pin_b::_0,
            typename pin_c::_0,
            typename pin_d::_1>;
        using stop = api::optimize<
            //
            typename pin_a::_0,
            typename pin_b::_0,
            typename pin_c::_0,
            typename pin_d::_0>;
        using gpio_cfg = api::optimize<api::mode<zoal::gpio::pin_mode::output, pin_a, pin_b, pin_c, pin_d>, stop>;

        static void select_step(uint8_t step) {
            switch (step) {
            case 0:
                step0();
                break;
            case 1:
                step1();
                break;
            case 2:
                step2();
                break;
            case 3:
                step3();
                break;
            case 4:
                step4();
                break;
            case 5:
                step5();
                break;
            case 6:
                step6();
                break;
            case 7:
                step7();
                break;
            default:
                stop();
                break;
            }
        }
    };

    template<class pin_a, class pin_b, class pin_c, class pin_d>
    class stepper_28byj_mode<4, pin_a, pin_b, pin_c, pin_d> {
    public:
        // pin_a - blue;
        // pin_b - pink;
        // pin_c - yellow;
        // pin_d - orage;

        using api = zoal::gpio::api;
        using step0 = api::optimize<api::high<pin_a, pin_b>, api::low<pin_c, pin_d>>;
        using step1 = api::optimize<api::high<pin_b, pin_c>, api::low<pin_a, pin_d>>;
        using step2 = api::optimize<api::high<pin_c, pin_d>, api::low<pin_a, pin_b>>;
        using step3 = api::optimize<api::high<pin_a, pin_d>, api::low<pin_b, pin_c>>;
        using stop = api::optimize<api::low<pin_a, pin_d, pin_b, pin_c>>;
        using gpio_cfg = api::optimize<api::mode<zoal::gpio::pin_mode::output, pin_a, pin_b, pin_c, pin_d>, stop>;

        static void select_step(uint8_t step) {
            switch (step) {
            case 0:
                step0();
                break;
            case 1:
                step1();
                break;
            case 2:
                step2();
                break;
            case 3:
                step3();
                break;
            default:
                stop();
                break;
            }
        }
    };

    template<class pin_a, class pin_b, class pin_c, class pin_d, uint8_t spr = 8>
    class stepper_28byj {
    public:
        static constexpr uint8_t steps_per_rotation = spr;
        static constexpr uint8_t mask = steps_per_rotation - 1;

        typedef enum { finished, idle, waiting, rotated } status;

        using stepper_mode = stepper_28byj_mode<spr, pin_a, pin_b, pin_c, pin_d>;
        using gpio_cfg = typename stepper_mode::gpio_cfg;

        void step_allowed() {
            perform_step = true;
        }

        status step_now() {
            perform_step = true;
            return step_if_needed();
        }

        status step_if_needed() {
            if (steps_left == 0) {
                stop();
                return idle;
            }

            if (perform_step) {
                perform_step = false;
                steps_left--;
                current_step = (current_step + (direction == 0 ? 1 : mask)) & mask;

                stepper_mode::select_step(current_step);
                return steps_left == 0 ? finished : rotated;
            }

            return steps_left == 0 ? finished : waiting;
        }

        void stop() {
            perform_step = false;
            steps_left = 0;

            zoal::gpio::api::optimize<zoal::gpio::api::low<pin_a, pin_d, pin_b, pin_c>>();
        }

        void rotate(uint32_t steps, uint8_t dir) {
            steps_left = steps;
            direction = dir;
        }

        uint32_t steps_left{0};
        uint8_t direction = 0;
        uint8_t current_step{0};
        volatile bool perform_step{false};
    };
}}

#endif
