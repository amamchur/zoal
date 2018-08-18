#pragma once

#ifndef ZOAL_IO_MEM_STREAM_HPP
#define ZOAL_IO_MEM_STREAM_HPP

#include <stdint.h>
#include <stddef.h>

namespace zoal { namespace io {
	template<uint8_t *Buffer>
	class mem_stream {
	public:
		static void write(uint8_t data) {
			Buffer[position_++] = data;
		}

		template<class F>
		static void write(F &fn) {
			uint8_t data = 0;
			while (fn(data)) {
				write(data);
			}
		}

		static void reset() {
			position_ = 0;
		}
		
		static void close() {
			write(0);
		}

		static size_t position() {
			return position_;
		}
	private:
		static size_t position_;
	};

	template<uint8_t *Buffer>
	size_t mem_stream<Buffer>::position_ = 0;
}}

#endif
