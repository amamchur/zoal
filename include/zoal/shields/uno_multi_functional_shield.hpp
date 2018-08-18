#ifndef ZOAL_SHIELDS_UNO_MULTI_FUNCTIONAL_SHIELD_HPP
#define ZOAL_SHIELDS_UNO_MULTI_FUNCTIONAL_SHIELD_HPP

#include "../gpio/pin.hpp"
#include "zoal/ic/ic74hc595.hpp"
#include "../io/button.hpp"
#include "../data/segment7.hpp"

namespace zoal { namespace shields {
	template<class Board, class Tools, void (*ButtonCallback)(uint8_t, ::zoal::io::button_event) = nullptr>
	class uno_multi_functional_shield_buttons {
	protected:
		typedef uno_multi_functional_shield_buttons<Board, Tools, ButtonCallback> self;
		typedef typename Tools::Counter Counter;

		void init_buttons() {
			button1.begin();
			button2.begin();
			button3.begin();
		}

		void handle_buttons() {
			button1.handle(this, &self::button1_handler);
			button2.handle(this, &self::button2_handler);
			button3.handle(this, &self::button3_handler);
		}

		void button1_handler(::zoal::io::button_event event) {
			ButtonCallback(0, event);
		}

		void button2_handler(::zoal::io::button_event event) {
			ButtonCallback(1, event);
		}

		void button3_handler(::zoal::io::button_event event) {
			ButtonCallback(2, event);
		}

		::zoal::io::button_ext<typename Board::BA01, Counter> button1;
		::zoal::io::button_ext<typename Board::BA02, Counter> button2;
		::zoal::io::button_ext<typename Board::BA03, Counter> button3;
	};

	template<class Board, class Tools>
	class uno_multi_functional_shield_buttons<Board, Tools, nullptr> {
	protected:
		inline void init_buttons() {
		}

		inline void handle_buttons() {
		}
	};

	template<class Board, class Tools, void (*ButtonCallback)(uint8_t, ::zoal::io::button_event) = nullptr>
	class uno_multi_functional_shield : public uno_multi_functional_shield_buttons<Board, Tools, ButtonCallback> {
	public:
		typedef uno_multi_functional_shield<Board, Tools, ButtonCallback> self;
		typedef typename Tools::Delay delay;

		typedef ::zoal::gpio::active_low<typename Board::BD13> Led1;
		typedef ::zoal::gpio::active_low<typename Board::BD12> Led2;
		typedef ::zoal::gpio::active_low<typename Board::BD11> Led3;
		typedef ::zoal::gpio::active_low<typename Board::BD10> Led4;
		typedef ::zoal::gpio::active_drain<typename Board::BD03> Beeper;
		typedef ::zoal::ic::ic74hc595<typename Board::BD08, typename Board::BD04, typename Board::BD07> Display;

		void begin() {
			this->init_buttons();

			Led1::off();
			Led2::off();
			Led3::off();
			Led4::off();
			Beeper::off();

			segmentDisplay.begin();
		}

		void beep(uint32_t duration = 50) {
			Beeper::on();
			delay::milliseconds(duration);
			Beeper::off();
		}

		void hexToSegments(uint16_t value) {
			uint16_t v = value;
			for (int i = 3; i >= 0; i--) {
				auto d = static_cast<uint8_t>(v & 0x0Fu);
				segments[i] = ~::zoal::data::segment7::abcd_hex(d);
				v >>= 4;
			}
		}

		void dynamicIndication() {
			auto segmentMask = static_cast<uint8_t>(1u << segmentIndex);
			auto segmentValue = segments[segmentIndex];
			segmentDisplay << segmentValue << segmentMask;
			segmentIndex = static_cast<uint8_t>((segmentIndex + 1) & 0x3);
		}

		void handle() {
			this->handle_buttons();
		}

		void display(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3) {
			segments[0] = s0;
			segments[1] = s1;
			segments[2] = s2;
			segments[3] = s3;
		}
	protected:
		Display segmentDisplay;
		uint8_t segments[4]{0, 0, 0, 0};
		uint8_t segmentIndex{0};
	};
}}

#endif
