#ifndef ZOAL_DATA_DATE_TIME_HPP
#define ZOAL_DATA_DATE_TIME_HPP

#include <stdint.h>

namespace zoal { namespace data {
	class date_time {
	public:
        uint8_t seconds{0};
		uint8_t minutes{0};
		uint8_t hours{0};
		uint8_t date{1};
		uint8_t day{1};
		uint8_t month{0};
		uint16_t year{1970};
	};
}}

#endif
