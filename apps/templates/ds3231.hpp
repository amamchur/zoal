#ifndef ZOAL_DS3231_HPP
#define ZOAL_DS3231_HPP

#include <zoal/ic/ds3231.hpp>

template<class Tools, class InterIntegratedCircuit>
class ds3231 {
public:
    using self_type = ds3231<Tools, InterIntegratedCircuit>;
    using tools = Tools;
    using logger = typename tools::logger;
    using i2c = InterIntegratedCircuit;
    using rtc_type = zoal::ic::ds3231<>;

    rtc_type rtc;

    typename tools::counter_value_type next_update{0};

    void init() {
        next_update = tools::counter::now();
        //        rtc.fetch();
        //        while (!rtc.ready) continue;
        //        auto currentDateTime = rtc.date_time();
        //        currentDateTime.year = 2018;
        //        currentDateTime.month = 11;
        //        currentDateTime.date = 20;
        //        currentDateTime.day = 2;
        //        currentDateTime.hours = 16;
        //        currentDateTime.minutes = 40;
        //        currentDateTime.seconds = 0;
        //        rtc.date_time(currentDateTime);
        //        rtc.update();
        //        while (!rtc.ready) continue;
    }

    void run_once() {
        if (next_update > tools::counter::now()) {
            return;
        }

//        rtc.fetch();

        while (!rtc.ready_()) continue;

        next_update = tools::counter::now() + 1000;
        auto dt = rtc.date_time();
        logger::info() << (int) dt.year << "-" << (int) dt.month << "-" << (int) dt.date << "T" << (int) dt.hours << ":"
                       << (int) dt.minutes << ":" << (int) dt.seconds;
    }

    void run() {
        while (true) {
            run_once();
        }
    }
};

#endif
