const Avr = require('./Avr');

class ATmega extends Avr {
    buildTimerList() {
        let result = [];
        for (let i = 0; i < this.mcu.timers.length; i++) {
            let t = this.mcu.timers[i];
            let hex = ATmega.toHex(t.address, 4);
            let m = t.module;

            // let regs = this.collectRegisters(m, /GTCCR|TIFR\d|TIMSK\d|ASSR/);
            // let word = t.type === 'timer16' ? 'uint16_t' : 'uint8_t';
            // let memModelName = `${t.name}_mem_model`;
            // result = result.concat(this.buildMemoryModel(memModelName, regs, word));

            if (t.type === 'timer16') {
                result.push(`using ${t.name} = ::zoal::arch::avr::atmega::timer16<${hex}, ${t.sn}>;`);
            } else {
                let async = m.$.name.match(/ASYNC/i) != null;
                result.push(`using ${t.name} = ::zoal::arch::avr::atmega::timer8<${hex}, ${t.sn}, ${async}>;`);
            }
        }
        return result;
    }

    buildUSARTList() {
        let result = [];
        for (let i = 0; i < this.mcu.usarts.length; i++) {
            let u = this.mcu.usarts[i];
            let hex = ATmega.toHex(u.address, 4);
            result.push(``);
            result.push(`template<class Buffer>`);
            result.push(`using ${u.name} = typename ::zoal::arch::avr::usart<${hex}, ${u.sn}, Buffer>;`);
        }
        return result;
    }

    buildADCList() {
        let result = [];
        let adc = this.mcu.adcs[0];
        let hex = ATmega.toHex(adc.address, 4);
        result.push(`using adc_00 = ::zoal::arch::avr::atmega::adc<${hex}, 0>;`);
        return result;
    }

    buildUSARTSsMetadata() {
        let result = [];
        for (let i = 0; i < this.mcu.usarts.length; i++) {
            let usart = this.mcu.usarts[i];
            let usartHex = ATmega.toHex(usart.address, 4);
            let signals = usart.signals;

            result.push(`template<>`);
            result.push(`struct usart_mapping<${usartHex}, 0x0000, 0> : base_usart_mapping<0, 0, 0> {};`);
            result.push(``);

            for (let j = 0; j < signals.length; j++) {
                let s = signals[j];
                let portHex = ATmega.toHex(s.port.address, 4);
                let rx = s.group === 'RXD' ? 0 : -1;
                let tx = s.group === 'TXD' ? 0 : -1;
                let ck = s.group === 'XCK' ? 0 : -1;
                if (rx < 0 && tx < 0 && ck < 0) {
                    continue;
                }

                result.push(``);
                result.push(`template<>`);
                result.push(`struct usart_mapping<${usartHex}, ${portHex}, ${s.offset}> : base_usart_mapping<${rx}, ${tx}, ${ck}> {`);
                result.push(`};`);
            }
        }
        return result;
    }

    buildClass() {
        let name = this.device.$.name;
        let nameUpper = name.toUpperCase();
        let nameLower = name.toLowerCase();

        this.buffer = [
            `/**`,
            ` * Autogenerated file. Do not modify manually.`,
            ` */`,
            `#ifndef ZOAL_MCU_${nameUpper}_HPP`,
            `#define ZOAL_MCU_${nameUpper}_HPP`,
            ``,
            `#include <stdint.h>`,
            `#include <zoal/arch/avr/atmega/adc.hpp>`,
            `#include <zoal/arch/avr/atmega/cfg.hpp>`,
            `#include <zoal/arch/avr/atmega/irq.hpp>`,
            `#include <zoal/arch/avr/atmega/metadata.hpp>`,
            `#include <zoal/arch/avr/atmega/mux.hpp>`,
            `#include <zoal/arch/avr/atmega/usart.hpp>`,
            `#include <zoal/arch/avr/port.hpp>`,
            `#include <zoal/arch/avr/atmega/timer16.hpp>`,
            `#include <zoal/arch/avr/atmega/timer8.hpp>`,
            `#include <zoal/arch/power.hpp>`,
            `#include <zoal/arch/enable.hpp>`,
            `#include <zoal/gpio/base_api.hpp>`,
            `#include <zoal/gpio/pin.hpp>`,
            `#include <zoal/gpio/port_link.hpp>`,
            `#include <zoal/mcu/base_mcu.hpp>`,
            ``,
            `namespace zoal { namespace mcu {`,
            `    template<uint32_t Frequency>`,
            `    class ${nameLower} : public base_mcu<Frequency, 1> {`,
            `    public:`,
            `    template<uintptr_t Address, uint8_t PinMask>`,
            `    using port = typename ::zoal::arch::avr::port<Address, PinMask>;`,
            `    `,
            this.buildPortList().join('\n'),
            ``,
            this.buildTimerList().join('\n'),
            ``,
            this.buildUSARTList().join('\n'),
            ``,
            this.buildADCList().join('\n'),
            ``,
            this.buildPinList().join('\n'),
            ``,
            this.buildPortChain().join('\n'),
            `    using api = ::zoal::gpio::base_api<port_chain>;`,
            `    using mux = ::zoal::arch::avr::atmega::mux<api>;`,
            `    using cfg = ::zoal::arch::avr::atmega::cfg<api, Frequency>;`,
            `    using irq = ::zoal::arch::avr::atmega::irq;`,
            ``,
            `    template<class ... Module>`,
            `    using power = ::zoal::arch::power<Module...>;`,
            ``,
            `    template<class ... Module>`,
            `    using enable = ::zoal::arch::enable<Module...>;`,
            `    };`,
            `}}`,
            ``,
            'namespace zoal { namespace metadata {',
            '    using zoal::utils::integral_constant;',
            '',
            this.buildTimersMetadata().join('\n'),
            '',
            this.buildUSARTSsMetadata().join('\n'),
            '',
            this.buildADCsMetadata().join('\n'),
            `}}`,
            '',
            `#endif`,
            ``
        ].join('\n');
    }
}

module.exports = ATmega;
