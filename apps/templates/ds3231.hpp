#ifndef ZOAL_DS3231_HPP
#define ZOAL_DS3231_HPP

#include <zoal/ic/ds3231.hpp>

template<class Tools, class iic>
class ds3231 {
public:
    using tools = Tools;
    using logger = typename tools::logger;
    using i2c = iic;
    using rtc_type = zoal::ic::ds3231<i2c>;

    rtc_type rtc;

    void init() {
        //    rtc.fetch();
        //    while (!rtc.ready) continue;
        //    auto currentDateTime = rtc.date_time();
        //    currentDateTime.year = 2018;
        //    currentDateTime.month = 9;
        //    currentDateTime.date = 20;
        //    currentDateTime.day = 4;
        //    currentDateTime.hours = 10;
        //    currentDateTime.minutes = 35;
        //    currentDateTime.seconds = 0;
        //    rtc.date_time(currentDateTime);
        //    rtc.update();
        //    while (!rtc.ready) continue;
    }

    void run_once() {
        rtc.fetch();
        while (!rtc.ready) continue;

        auto dt = rtc.date_time();
        logger::info() << (int)dt.year << "-" << (int)dt.month << "-" << (int)dt.date << "T" << (int)dt.hours << ":"
                       << (int)dt.minutes << ":" << (int)dt.seconds;
    }

    void run() {
        while (true) {
            run_once();
        }
    }
};

#endif
