const fs = require('fs');
const xml2js = require('xml2js');
const Avr = require('./Avr');
let exec = require('child_process').exec;
let path = require('path');

class ATtiny extends Avr {
    buildADCList() {
        let adc = this.mcu.adcs[0];
        if (!adc) {
            return [];
        }

        let regs = this.collectRegisters(adc.module);
        let adcName = 'adc_00';

        let result = [
            `class ${adcName}_mem_model {`,
            `public:`
        ];

        let address = 0xFFFF;
        for (let i = 0; i < regs.array.length; i++) {
            let r = regs.array[i];
            let offsetHex = Avr.toHex(r.offset, 2);
            address = Math.min(r.address, address);
            result.push(`static constexpr uintptr_t ${r.avrName}x = ${offsetHex};`);
        }

        result.push(`};`);
        result.push(``);

        adc.address = address;
        let hex = Avr.toHex(address, 4);
        result.push(`using adc_00 = ::zoal::arch::avr::adc<${hex}, 0, ${adcName}_mem_model>;`);
        return result;
    }

    buildTimerList() {
        let result = [];

        for (let i = 0; i < this.mcu.timers.length; i++) {
            let t = this.mcu.timers[i];
            let m = t.module;
            let regs = this.collectRegisters(m, /GTCCR/);
            let word = t.type === 'timer16' ? 'uint16_t' : 'uint8_t';
            let memModelName = `${t.name}_mem_model`;

            result = result.concat(this.buildMemoryModel(memModelName, regs, word));

            t.address = regs.array[0].address;

            let hex = Avr.toHex(t.address, 4);
            if (t.type === 'timer16') {
                result.push(`using ${t.name} = ::zoal::arch::avr::timer16<${hex}, ${t.sn}, ${t.name}_mem_model>;`);
            } else {
                let async = m.$.name.match(/ASYNC/i) != null;
                result.push(`using ${t.name} = ::zoal::arch::avr::timer8<${hex}, ${t.sn}, ${async}, ${t.name}_mem_model>;`);
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
            `#include <zoal/arch/avr/port.hpp>`,
            // `#include <zoal/arch/avr/attiny/timer16.hpp>`,
            // `#include <zoal/arch/avr/attiny/timer8.hpp>`,
            `#include <zoal/arch/avr/attiny/cfg.hpp>`,
            `#include <zoal/arch/avr/attiny/irq.hpp>`,
            `#include <zoal/arch/avr/attiny/metadata.hpp>`,
            `#include <zoal/arch/avr/attiny/mux.hpp>`,
            `#include <zoal/arch/avr/adc.hpp>`,
            `#include <zoal/arch/avr/timer8.hpp>`,
            `#include <zoal/arch/avr/timer16.hpp>`,
            `#include <zoal/gpio/base_api.hpp>`,
            `#include <zoal/gpio/pin.hpp>`,
            `#include <zoal/gpio/port_link.hpp>`,
            `#include <zoal/mcu/base_mcu.hpp>`,
            `#include <zoal/arch/power.hpp>`,
            `#include <zoal/arch/enable.hpp>`,
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
            this.buildADCList().join('\n'),
            ``,
            this.buildPinList().join('\n'),
            ``,
            this.buildPortChain().join('\n'),
            `    using api = ::zoal::gpio::base_api<port_chain>;`,
            `    using mux = ::zoal::arch::avr::attiny::mux<api>;`,
            `    using cfg = ::zoal::arch::avr::attiny::cfg<api, Frequency>;`,
            `    using irq = ::zoal::arch::avr::attiny::irq;`,
            ``,
            `    template<class ... Module>`,
            `    using power = ::zoal::arch::power<Module...>;`,
            ``,
            `    template<class ... Module>`,
            `    using enable = ::zoal::arch::enable<Module...>;`,
            `};`,
            `}}`,
            ``,
            'namespace zoal { namespace metadata {',
            '    using zoal::utils::integral_constant;',
            '',
            this.buildTimersMetadata().join('\n'),
            '',
            this.buildADCsMetadata().join('\n'),
            `}}`,
            '',
            `#endif`,
            ``
        ].join('\n');
    }
}

module.exports = ATtiny;