#ifndef APPS_IRREMOVEAPP_HPP_
#define APPS_IRREMOVEAPP_HPP_

#include <zoal/io/ir_remote_receiver.hpp>
#include <zoal/io/output_stream.hpp>

template<class Pin, class Tools, uint32_t p>
class ir_remove {
public:
    static constexpr uint32_t period_microseconds = p;

    using delay = typename Tools::delay;
    using logger = typename Tools::logger;
    using receiver_type = zoal::io::ir_remote_receiver<Pin, period_microseconds>;

    void init() {
        receiver.begin();
    }

    void handlePeriod() {
        receiver.handle();
    }

    void run_once() {
        using namespace zoal::io;

        if (receiver.processed()) {
            logger::info() << hex << receiver.result();
            receiver.run();
        }

        delay::template ms<period_microseconds>();
    }

    void run() {
        logger::info() << "IRRemoveApp Started";

        while (true) {
            run_once();
        }
    }

private:
    receiver_type receiver;
};

#endif
