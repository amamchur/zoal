#ifndef ZOAL_ADC_REQUEST_DISPATCHER_HPP
#define ZOAL_ADC_REQUEST_DISPATCHER_HPP

#include <stdint.h>
#include <zoal/func/function.hpp>
#include <zoal/utils/interrupts.hpp>

namespace zoal { namespace periph {
    enum class adc_request_status { idle, pending, finished };

    template<class Adc>
    class adc_dispatcher;

    template<class Adc, size_t ClosureBankSize>
    class adc_api_dispatcher {
    public:
        using adc = Adc;
        using value_type = uint16_t;
        using client_callback_type = zoal::func::function<ClosureBankSize, void, value_type>;

        template<class Channel, class Callback>
        bool fetch(Callback callback) {
            zoal::utils::interrupts_off off;
            if (status != zoal::periph::adc_request_status::idle) {
                return false;
            }

            status = adc_request_status::pending;
            client_callback.assign(callback);
            typename Channel::connect();
            adc::start();
            return true;
        }

        inline void complete(uint32_t v) {
            value = v;
            status = adc_request_status::finished;
        }

        inline volatile bool finished() const {
            switch (status) {
            case adc_request_status::idle:
            case adc_request_status::pending:
                return false;
            default:
                return true;
            }
        }

        inline bool processing() const {
            return status != zoal::periph::adc_request_status::idle;
        }

        void handle() {
            if (!finished()) {
                return;
            }

            if (client_callback) {
                typeof(client_callback) cb(client_callback);
                client_callback.reset();
                status = zoal::periph::adc_request_status::idle;
                cb(value);
            }
        }

        void handle_until_finished() {
            do {
                handle();
            } while (processing());
        }

    private:
        client_callback_type client_callback;
        value_type value;
        volatile zoal::periph::adc_request_status status{zoal::periph::adc_request_status::idle};
    };

    template<class Adc, size_t ClosureBankSize>
    class adc_dispatcher_base {
    public:
        static adc_api_dispatcher<Adc, ClosureBankSize> api;
    };

    template<class Adc, size_t ClosureBankSize>
    adc_api_dispatcher<Adc, ClosureBankSize> adc_dispatcher_base<Adc, ClosureBankSize>::api;
}}

#endif
