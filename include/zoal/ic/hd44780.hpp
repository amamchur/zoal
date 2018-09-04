#ifndef ZOAL_IC_HD44780_HPP
#define ZOAL_IC_HD44780_HPP

#include <stdint.h>
#include "../gpio/pin_mode.hpp"
#include "../io/stream.hpp"

namespace zoal { namespace ic {
	template<class Tools, class RS, class EN,
		class D4, class D5, class D6, class D7>
	class hd44780_interface_4bit {
	public:
		using tools = Tools;
		using api = typename tools::api;
		using delay = typename tools::delay;
		using chain = typename api::chain;

		static constexpr uint8_t bit_mode = 4;

		static void init() {
			using namespace zoal::gpio;

			api::power_on(chain()
				& RS() & EN()
				& D4() & D5() & D6() & D7()
			);

			api::template mode<pin_mode::output_push_pull>(chain()
				& RS() & EN()
				& D4() & D5() & D6() & D7()
			);

			api::low(chain()
				& RS() & EN()
				& D6() & D7()
			);

			tools::api::high(typename tools::api::chain() & D4() & D5());

			pulseEnable();
			delay::ms(5);

			pulseEnable();
			delay::template us<100>();
			
			D4::low();
			pulseEnable();
			delay::template us<37>();
		}

		static void send(uint8_t value, uint8_t rs) {
			RS::write(rs);

			D4::write((value >> 4) & 1);
			D5::write((value >> 5) & 1);
			D6::write((value >> 6) & 1);
			D7::write((value >> 7) & 1);
			pulseEnable();

			D4::write((value >> 0) & 1);
			D5::write((value >> 1) & 1);
			D6::write((value >> 2) & 1);
			D7::write((value >> 3) & 1);
			pulseEnable();
		}
	protected:
		static void pulseEnable() {
			EN::low();
			delay::template us<1>();
			EN::high();
			delay::template us<1>();
			EN::low();
		}
	};

	template<class Tools, class RS, class EN,
		class D0, class D1, class D2, class D3, 
		class D4, class D5, class D6, class D7>
	class hd44780_interface_8bit {
	protected:
		static void pulseEnable() {
			EN::high();
			Tools::Delay::template microseconds<4>();
			EN::low();
		}
	public:
		static constexpr uint8_t bit_mode = 8;

		static void init() {
			using namespace zoal::gpio;

			Tools::API::template mode<pin_mode::output_push_pull>(
				Tools::API::Chain()
				& RS() & EN()
				& D0() & D1() & D2() & D3()
				& D4() & D5() & D6() & D7()
			);

			Tools::API::low(Tools::API::Chain() & D7() & D6() & RS() & EN());
			Tools::API::high(Tools::API::Chain() & D5() & D4());

			pulseEnable();
			Tools::Delay::template microseconds<5>();

			pulseEnable();
			Tools::Delay::template microseconds<100>();

			pulseEnable();
			Tools::Delay::template microseconds<37>();
		}

		static void send(uint8_t value, uint8_t rs) {
			RS::write(rs);

			D0::write((value >> 0) & 1);
			D1::write((value >> 1) & 1);
			D2::write((value >> 2) & 1);
			D3::write((value >> 3) & 1);
			D4::write((value >> 4) & 1);
			D5::write((value >> 5) & 1);
			D6::write((value >> 6) & 1);
			D7::write((value >> 7) & 1);
			pulseEnable();
		}
	};

	template<uint8_t columns, uint8_t rows>
	class hd44780_address_selector {
	public:
		static constexpr uint8_t Columns = columns;
		static constexpr uint8_t Rows = rows;

		static uint8_t getAddress(uint8_t row, uint8_t column) {
			uint8_t addr;
			switch (row) {
				case 0: 
					addr = 0x00;
					break;
				case 1: 
					addr = 0x40;
					break;
				case 2:
					addr = columns;
					break;
				default: 
					addr = static_cast<uint8_t>(0x40 + columns);
					break;
			}

			return addr + column;
		}
	};

	template<class Interface, class AddressSelector>
	class hd44780 {
	public:
		using delay = typename Interface::tools::delay;

		enum : uint8_t {
			CmdClearDisplay = 0x01,
			CmdCursorHome 	= 0x02,
			CmdEntryModeSet	= 0x04,
			CmdDisplayCtrl 	= 0x08,
			CmdFunctionSet 	= 0x20,
			CmdSetCGRAMAddr = 0x40,
			CmdSetDDRAMAddr = 0x80
		};

		using Class = hd44780<Interface, AddressSelector>;
		
		hd44780() = delete;

		static void init() {
			Interface::init();

			auto cfg = static_cast<uint8_t>(Interface::bit_mode == 8 ? 0x10 : 0);
			cfg |= AddressSelector::Rows == 1 ? 0x00 : 0x08;

			Interface::send(CmdFunctionSet | cfg, 0);
			delay::template us<37>();

			display();
			clear();
		}

		static void clear() {
			Interface::send(CmdClearDisplay, 0);
			delay::ms(5);
		}

		static void home() {
			Interface::send(CmdCursorHome, 0);
			delay::milliseconds(2);
		}

		static void display(bool on = true, bool cursor = false, bool blink = false) {
			uint8_t cmd = CmdDisplayCtrl;
			if (on) {
				cmd |= 1 << 2;
			}

			if (cursor) {
				cmd |= 1 << 1;
			}

			if (blink) {
				cmd |= 1;
			}

			Interface::send(cmd, 0);
			delay::template us<37>();
		}

		static void move(uint8_t row, uint8_t column) {
			uint8_t cmd = CmdSetDDRAMAddr;
			cmd |= AddressSelector::getAddress(row, column);
			Interface::send(cmd, 0);
			delay::template us<37>();
		}

		static void ddram_addr(uint8_t addr) {
			uint8_t cmd = CmdSetDDRAMAddr + addr;
			Interface::send(cmd, 0);
			delay::template us<37>();
		}

		static void write(uint8_t charCode) {
			Interface::send(charCode, 1);
			delay::template us<37>();
		}

		static void write(const char *str) {
			while (*str) {
				Interface::send(*str++, 1);
				delay::template us<37>();
			}
		}

		template<class F>
		static void write(F &fn) {
			uint8_t data = 0;
			while (fn(data))  {
				write(data);
			}
		}

		static void create_char(uint8_t pos, const uint8_t *data) {
			uint8_t cmd = CmdSetCGRAMAddr | pos << 3;
			Interface::send(cmd, 0);
			delay::template us<37>();

			for (int i = 0; i < 8; i++) {
				write(data[i]);
			}
		}

		static void entry_mode(bool increment = true, bool shift = false) {
  			uint8_t cmd = CmdEntryModeSet;
			if (increment) {
				cmd |= 2;
			}

			if (shift) {
				cmd |= 1;
			}
			Interface::send(cmd, 0);
			delay::template us<37>();
		}
	};
}}

#endif
