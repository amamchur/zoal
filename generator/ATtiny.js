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
            "template<uint32_t Address>",
            `class ${adcName}_mem_model {`,
            `public:`
        ];

        let address = 0xFFFF;
        for (let i = 0; i < regs.array.length; i++) {
            let r = regs.array[i];
            let offsetHex = Avr.toHex(r.offset, 2);
            address = Math.min(r.address, address);
            // result.push(`static constexpr uintptr_t ${r.avrName}x = ${offsetHex};`);
            result.push(`using ${r.avrName}x= zoal::mem::reg<Address + ${offsetHex}, zoal::mem::reg_io::read_write, uint8_t, 0xFF>;`);
        }

        result.push(`};`);
        result.push(``);

        adc.address = address;
        let hex = Avr.toHex(address, 4);
        result.push(`using adc_00 = ::zoal::arch::avr::adc<${hex}, 0, ${adcName}_mem_model<${hex}>>;`);
        result.push(``);
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
                result.push(`using ${t.name} = ::zoal::arch::avr::timer16<${hex}, ${t.sn}, ${t.name}_mem_model<${hex}>>;`);
            } else {
                let async = m.$.name.match(/ASYNC/i) != null;
                result.push(`using ${t.name} = ::zoal::arch::avr::timer8<${hex}, ${t.sn}, ${async}, ${t.name}_mem_model<${hex}>>;`);
            }

            result.push(``);
        }
        return result;
    }

    buildClass() {
        let name = this.device.$.name;
        let nameUpper = name.toUpperCase();
        let nameLower = name.toLowerCase();
        let sign = this.build_signature(name);
        let sign_name = `${nameLower}_sign`;

        this.buffer = [
            `/**`,
            ` * Autogenerated file. Do not modify manually.`,
            ` */`,
            `#ifndef ZOAL_MCU_${nameUpper}_HPP`,
            `#define ZOAL_MCU_${nameUpper}_HPP`,
            ``,
            `#include <stdint.h>`,
            `#include <zoal/arch/avr/port.hpp>`,
            `#include <zoal/arch/avr/attiny/cfg.hpp>`,
            `#include <zoal/arch/avr/attiny/irq.hpp>`,
            `#include <zoal/arch/avr/attiny/metadata.hpp>`,
            `#include <zoal/arch/avr/attiny/mux.hpp>`,
            `#include <zoal/arch/avr/adc.hpp>`,
            `#include <zoal/arch/avr/timer8.hpp>`,
            `#include <zoal/arch/avr/timer16.hpp>`,
            `#include <zoal/gpio/api.hpp>`,
            `#include <zoal/gpio/pin.hpp>`,
            `#include <zoal/arch/power.hpp>`,
            `#include <zoal/arch/enable.hpp>`,
            `#include <zoal/ct/type_list.hpp>`,
            `#include <zoal/ct/signature.hpp>`,
            ``,
            `namespace zoal { namespace mcu {`,
            `    class ${nameLower} {`,
            `    public:`,
            `       using self_type = ${nameLower};`,
            `       using signature = ${sign};`,
            '',
            `    template<uintptr_t Address, uint8_t PinMask>`,
            `    using port = typename ::zoal::arch::avr::port<Address, PinMask>;`,
            ``,
            this.buildPortList().join('\n'),
            ``,
            this.buildTimerList().join('\n'),
            ``,
            this.buildADCList().join('\n'),
            ``,
            this.buildPinList().join('\n'),
            ``,
            this.buildPortChain().join('\n'),
            `    using api = ::zoal::gpio::api;`,
            '    using mux = ::zoal::arch::avr::attiny::mux<self_type>;',
            '    using cfg = ::zoal::arch::avr::attiny::cfg<self_type>;',
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
            '    using zoal::ct::integral_constant;',
            `    using ${sign_name} = ${sign};`,
            '',
            this.buildTimersMetadata(sign_name).join('\n'),
            '',
            this.buildADCsMetadata(sign_name).join('\n'),
            `}}`,
            '',
            `#endif`,
            ``
        ].join('\n');
    }
}

module.exports = ATtiny;
