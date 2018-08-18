#ifndef ZOAL_IO_INPUT_STREAM_HPP
#define ZOAL_IO_INPUT_STREAM_HPP

#include <ctype.h>
#include <stdint.h>
#include "../parser/number.hpp"
#include "stream.hpp"

namespace zoal { namespace io {
	template <class Transport>
	class input_stream {
	public:
		input_stream() : success(true), radix(10) {
		}

		operator bool() const {
			return success;
		}

		void clear() {
			success = true;
		}

		void ignore(int count = 1, char ch = '\n') {
			ignore_functor fn(count, ch);
			bool done;
			do {
				done = Transport::read(fn);
			} while (!done);
		}

		char peek() {
			char buffer[1] = {0};
			Transport::peek(buffer, sizeof(buffer));
			return buffer[0];
		}

		template<class P, class V>
		void parseValue(P &parser, V &value) {
			using namespace zoal::parser;
			while (parser.state != integer_parse_state::done && parser.state != integer_parse_state::error) {
				Transport::read(parser);
			}

			switch (parser.state) {
				case integer_parse_state::done:
					value = parser;
					break;
				default:
					success = false;
				break;
			}
		}

		input_stream& operator >>(uint8_t &value) {
			using namespace zoal::parser;
			number parser(radix);
			parseValue<number, uint8_t>(parser, value);
			return *this;
		}

		input_stream& operator >>(uint16_t &value) {
			using namespace zoal::parser;
			number parser(radix);
			parseValue<number, uint16_t>(parser, value);
			return *this;
		}

		input_stream& operator >>(uint32_t &value) {
			using namespace zoal::parser;
			number parser(radix);
			parseValue<number, uint32_t>(parser, value);
			return *this;
		}

		input_stream& operator >>(int8_t &value) {
			using namespace zoal::parser;
			number parser(radix);
			parseValue<number, int8_t>(parser, value);
			return *this;
		}

		input_stream& operator >>(int16_t &value) {
			using namespace zoal::parser;
			number parser(radix);
			parseValue<number, int16_t>(parser, value);
			return *this;
		}

		input_stream& operator >>(int32_t &value) {
			using namespace zoal::parser;
			number parser(radix);
			parseValue<number, int32_t>(parser, value);
			return *this;
		}

		template <class F>
		input_stream& operator >>(F fn) {
			Transport::read(fn);
			return *this;
		}

		template <uint8_t v>
		input_stream& operator<< (const radix_value<v> &) {
			this->radix = 8;
			return *this;
		}

		bool success;
		uint8_t radix;
	};
}}

#endif
