#ifndef ZOAL_DATA_DATE_TIME_HPP
#define ZOAL_DATA_DATE_TIME_HPP

#include <stdint.h>

namespace zoal { namespace data {
    // According to international standard ISO 8601, Monday is the first day of the week
    enum day_of_week {
        monday = 1,
        tuesday,
        wednesday,
        thursday,
        friday,
        saturday,
        sunday
    };

	class date_time {
	public:
        uint8_t seconds{0};
		uint8_t minutes{0};
		uint8_t hours{0};
		uint8_t date{1};
        day_of_week day{thursday};
		uint8_t month{1};
		uint16_t year{1970};
	};
}}

#endif
