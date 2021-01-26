#ifndef ZOAL_PERIPH_I2C_REQUEST_DISPATCHER_HPP
#define ZOAL_PERIPH_I2C_REQUEST_DISPATCHER_HPP

#include "../func/function.hpp"
#include "./i2c_device.hpp"
#include "./i2c_request.hpp"

namespace zoal { namespace periph {
    template<class I2Circuit, class Function>
    class i2c_request_finisher {
    public:
        explicit i2c_request_finisher(Function &f)
            : function(f) {}
        i2c_request_finisher(const i2c_request_finisher &) = delete;
        i2c_request_finisher(const i2c_request_finisher &&cb) noexcept
            : function(cb.function) {}

        template<class T, class... Args>
        inline void operator()(T t, Args... args) {
            function.assign(t, args...);
            I2Circuit::start();
        }

        inline void operator()() {
            I2Circuit::start();
        }
    private:
        Function &function;
    };

    template<class I2Circuit, size_t ClosureBankSize>
    class i2c_request_dispatcher {
    public:
        using i2c = I2Circuit;
        using periph_callback_type = zoal::func::function<ClosureBankSize, void, i2c_request_dispatcher &>;
        using client_callback_type = zoal::func::function<ClosureBankSize, void>;
        using finisher_type = i2c_request_finisher<i2c, client_callback_type>;

        zoal::periph::i2c_request request;

        zoal::periph::i2c_request *acquire_request() {
            return &request;
        }

        finisher_type make_finisher() {
            return finisher_type(client_callback);
        }

        void finish_sequence() {
            success_periph_callback.reset();
            failed_periph_callback.reset();

            if (client_callback) {
                typeof(client_callback) cb(client_callback);
                cb();
            }
        }

        void handle() {
            if (!request.finished()) {
                return;
            }

            switch (request.status) {
            case zoal::periph::i2c_request_status::finished:
            case zoal::periph::i2c_request_status::finished_eos: {
                typeof(success_periph_callback) cb(success_periph_callback);
                success_periph_callback.reset();
                cb(*this);
                break;
            }
            default: {
                typeof(failed_periph_callback) cb(failed_periph_callback);
                failed_periph_callback.reset();
                cb(*this);
                break;
            }
            }

            if (!success_periph_callback && !failed_periph_callback) {
                request.reset();
                client_callback.reset();
            }

            if (request.status == zoal::periph::i2c_request_status::pending) {
                i2c::start();
            }
        }

        void handle_until_finished() {
            do {
                handle();
            } while (request.processing());
        }

    private:
        friend class i2c_device;

        periph_callback_type success_periph_callback;
        periph_callback_type failed_periph_callback;
        client_callback_type client_callback;
    };
}}

#endif
