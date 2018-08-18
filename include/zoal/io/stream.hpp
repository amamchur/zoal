#ifndef ZOAL_IO_STREAM_HPP
#define ZOAL_IO_STREAM_HPP

#include <stdint.h>
#include <stddef.h>
#include <ctype.h>

namespace zoal { namespace io {
	template <uint8_t Value>
	struct stream_value {
		static constexpr uint8_t value = Value;
	};

	template <uint8_t Value>
	struct radix_value {
		static constexpr uint8_t value = Value;
	};

	template <uint8_t Value>
	struct precision_value {
		static constexpr uint8_t value = Value;
	};

	struct setprecision {
		setprecision(uint8_t value) : value(value) {
		}

		uint8_t value;
	};

	struct cursor_position {
		cursor_position(uint8_t row, uint8_t col) : row(row), col(col) {
		}

		uint8_t row;
		uint8_t col;
	};

	struct abstract_stream_functor {
		virtual ~abstract_stream_functor() = default;

        virtual bool operator ()(uint8_t &data) = 0;
	};

	template<class T>
	struct stream_functor : public abstract_stream_functor {
        explicit stream_functor(T &t) : obj(t) {
		}

		bool operator ()(uint8_t &data) override {
			return obj.operator()(data);
		}

		T &obj;
	};

	struct new_line_cr_lf {
		bool operator ()(uint8_t &data) {
			data = static_cast<uint8_t>(index == 0 ? '\r' : '\n');
			return index++ < 2;
		}

		stream_functor<new_line_cr_lf> abstraction(){
			return stream_functor<new_line_cr_lf>(*this);
		}

		uint8_t index{0};
	};

	struct stop_escape_sequence {
		bool operator ()(uint8_t &data) {
			switch (index) {
				case 0: data = '\033'; break;
				case 1: data = '['; break;
				case 2: data = '0'; break;
				case 3: data = 'm'; break;
				default: break;
			}
			return index++ < 4;
		}

		stream_functor<stop_escape_sequence> abstraction(){
			return stream_functor<stop_escape_sequence>(*this);
		}

		uint8_t index{0};
	};

	struct fill_functor {
		fill_functor(uint8_t ch, size_t count) : ch(ch), count(count) {
		}

		bool operator ()(uint8_t &data) {
			data = ch;
			return count-- > 0;
		}

		stream_functor<fill_functor> abstraction(){
			return stream_functor<fill_functor>(*this);
		}

		uint8_t ch{' '};
		size_t count{0};
	};

	struct buffer_functor {
		buffer_functor(const void *buffer, size_t count)
			: buffer((const uint8_t *)buffer)
			, count(count) {
		}

		bool operator ()(uint8_t &data) {
			data = *buffer++;
			return count-- > 0;
		}

		stream_functor<buffer_functor> abstraction(){
			return stream_functor<buffer_functor>(*this);
		}

		const uint8_t *buffer;
		size_t count;
	};

	struct string_functor {
		explicit string_functor(const void *buffer)
			: buffer((const uint8_t *)buffer) {
		}

		bool operator ()(uint8_t &data) {
			data = *buffer++;
			return data != 0;
		}

		stream_functor<string_functor> abstraction(){
			return stream_functor<string_functor>(*this);
		}

		const uint8_t *buffer;
	};


	struct ignore_functor {
		ignore_functor(int count, char ch) : count(count), ch(ch) {
		}

		bool operator ()(uint8_t value) {
			return !(--count <= 0 || value == ch);
		}

		int count;
		char ch;
	};

	struct white_space_functor {
		bool operator ()(uint8_t value) {
			return isspace(value);
		}
	};

	template<int (*ptr)(int c)>
	class put_char_transport {
	public:
		static void write(uint8_t data) {
			ptr(data);
		}

		template<class F>
		static void write(F &fn) {
            uint8_t data = 0;
            while (fn(data))  {
                write(data);
            }
		}
	};

	typedef cursor_position pos;

	const stream_value<'\n'> endl;
	const radix_value<16> hex;
	const radix_value<10> dec;
	const radix_value<8> oct;
	const precision_value<0> fixed;
	const white_space_functor ws;
}}

#endif
