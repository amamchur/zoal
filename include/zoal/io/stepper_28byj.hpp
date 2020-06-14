#ifndef ZOAL_STEPPER_28BYJ_HPP
#define ZOAL_STEPPER_28BYJ_HPP

#include "../gpio/api.hpp"

namespace zoal { namespace io {
    template<int steps, class Tools, class pin_a, class pin_b, class pin_c, class pin_d>
    class stepper_28byj_mode {};

    template<class Tools, class pin_a, class pin_b, class pin_c, class pin_d>
    class stepper_28byj_mode<8, Tools, pin_a, pin_b, pin_c, pin_d> {
    public:
        using tools = Tools;

        static void init() {
            zoal::gpio::api::optimize<zoal::gpio::api::mode<zoal::gpio::pin_mode::output, pin_a, pin_b, pin_c, pin_d>>();
        }

        static void select_step(uint8_t step) {
            switch (step) {
            case 0:
                typename tools::api::template _1<pin_a, pin_b>();
                typename tools::api::template _0<pin_c, pin_d>();
                break;
            case 1:
                typename tools::api::template _1<pin_b>();
                typename tools::api::template _0<pin_a, pin_c, pin_d>();
                break;
            case 2:
                typename tools::api::template _1<pin_b, pin_c>();
                typename tools::api::template _0<pin_a, pin_d>();
                break;
            case 3:
                typename tools::api::template _1<pin_c>();
                typename tools::api::template _0<pin_a, pin_b, pin_d>();
                break;
            case 4:
                typename tools::api::template _1<pin_c, pin_d>();
                typename tools::api::template _0<pin_a, pin_b>();
                break;
            case 5:
                typename tools::api::template _1<pin_d>();
                typename tools::api::template _0<pin_a, pin_c, pin_b>();
                break;
            case 6:
                typename tools::api::template _1<pin_a, pin_d>();
                typename tools::api::template _0<pin_b, pin_c>();
                break;
            case 7:
                typename tools::api::template _1<pin_a>();
                typename tools::api::template _0<pin_b, pin_c, pin_d>();
                break;
            default:
                typename tools::api::template _0<pin_a, pin_d, pin_b, pin_c>();
                break;
            }
        }
    };

    template<class Tools, class pin_a, class pin_b, class pin_c, class pin_d>
    class stepper_28byj_mode<4, Tools, pin_a, pin_b, pin_c, pin_d> {
    public:
        using tools = Tools;
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

        static void init() {
            api::optimize<api::mode<zoal::gpio::pin_mode::output, pin_a, pin_b, pin_c, pin_d>>::apply();
        }

        static void select_step(uint8_t step) {
            switch (step) {
            case 0:
                step0::apply();
                break;
            case 1:
                step1::apply();
                break;
            case 2:
                step2::apply();
                break;
            case 3:
                step3::apply();
                break;
            default:
                stop::apply();
                break;
            }
        }
    };

    template<class Tools, class pin_a, class pin_b, class pin_c, class pin_d, uint8_t spr = 8>
    class stepper_28byj {
    public:
        static constexpr uint8_t steps_per_rotation = spr;
        static constexpr uint8_t mask = steps_per_rotation - 1;

        typedef enum { finished, idle, waiting, rotated } status;

        using tools = Tools;
        using stepper_mode = stepper_28byj_mode<spr, Tools, pin_a, pin_b, pin_c, pin_d>;

        void init() {
            stepper_mode::init();
        }

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

            zoal::gpio::api::optimize<zoal::gpio::api::low<pin_a, pin_d, pin_b, pin_c>>::apply();

            //            typename tools::api::template _0<pin_a, pin_d, pin_b, pin_c>();
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
