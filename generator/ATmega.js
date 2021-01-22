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
            result.push(`using ${u.name} = typename ::zoal::arch::avr::atmega::usart<${hex}>;`);
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

    buildSPIs() {
        let result = [];
        for (let i = 0; i < this.mcu.spis.length; i++) {
            let spi = this.mcu.spis[i];
            let hex = ATmega.toHex(spi.address, 4);
            result.push(`using spi_00 = ::zoal::arch::avr::atmega::spi<${hex}, ${i}>;`);
        }
        return result;
    }

    buildI2Cs() {
        let result = [];
        for (let i = 0; i < this.mcu.i2cs.length; i++) {
            let spi = this.mcu.i2cs[i];
            let hex = ATmega.toHex(spi.address, 4);
            result.push(``);
            result.push(`using i2c_00 = ::zoal::arch::avr::atmega::i2c<${hex}, ${i}>;`);
        }
        return result;
    }

    buildUSARTSsMetadata(signName) {
        let result = [];
        for (let i = 0; i < this.mcu.usarts.length; i++) {
            let usart = this.mcu.usarts[i];
            let usartHex = ATmega.toHex(usart.address, 4);
            let signals = usart.signals;

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
                result.push(`struct usart_mapping<${signName}, ${usartHex}, ${portHex}, ${s.offset}> : base_usart_mapping<${rx}, ${tx}, ${ck}> {`);
                result.push(`};`);
            }
        }
        return result;
    }

    buildSPIsMetadata(signName) {
        let result = [];
        for (let i = 0; i < this.mcu.spis.length; i++) {
            let spi = this.mcu.spis[i];
            let address = ATmega.toHex(spi.address, 4);
            let signals = spi.signals;

            for (let j = 0; j < signals.length; j++) {
                let s = signals[j];
                if (!s.group.match(/MISO|MOSI|SCK|SS/)) {
                    continue
                }

                let portHex = ATmega.toHex(s.port.address, 4);
                let miso = s.group === 'MISO' ? 0 : -1;
                let mosi = s.group === 'MOSI' ? 0 : -1;
                let clock = s.group === 'SCK' ? 0 : -1;
                let ss = s.group === 'SS' ? 0 : -1;

                result.push(``);
                result.push(`template<>`);
                result.push(`struct spi_mapping<${signName}, ${address}, ${portHex}, ${s.offset}> : base_spi_mapping<${mosi}, ${miso}, ${clock}, ${ss}> {`);
                result.push(`};`);
            }
        }

        return result;
    }


    buildI2CsMetadata(signName) {
        let result = [];
        for (let i = 0; i < this.mcu.i2cs.length; i++) {
            let spi = this.mcu.i2cs[i];
            let address = ATmega.toHex(spi.address, 4);
            let signals = spi.signals;

            for (let j = 0; j < signals.length; j++) {
                let s = signals[j];
                if (!s.group.match(/SDA|SCL/)) {
                    continue
                }

                let portHex = ATmega.toHex(s.port.address, 4);
                let sda = s.group === 'SDA' ? 0 : -1;
                let scl = s.group === 'SCL' ? 0 : -1;

                result.push(``);
                result.push(`template<>`);
                result.push(`struct i2c_mapping<${signName}, ${address}, ${portHex}, ${s.offset}> : base_i2c_mapping<${sda}, ${scl}> {`);
                result.push(`};`);
            }
        }

        return result;
    }

    buildPinAliases() {
        let result = ['// Pin aliases'];
        for (let i = 0; i < this.mcu.spis.length; i++) {
            let spi = this.mcu.spis[i];
            let signals = spi.signals;
            let moduleSuffix = '_' + ("00" + i.toString(10)).substr(-2);

            for (let j = 0; j < signals.length; j++) {
                let s = signals[j];
                if (!s.group.match(/MISO|MOSI|SCK|SS/)) {
                    continue
                }

                let name = s.group.toLowerCase() + moduleSuffix;
                let portName = s.port.name;
                let prefix = portName.replace(/port_(.)/, "p$1");
                let suffix = ("00" + s.offset.toString(16)).substr(-2);
                let pinName = `${prefix}_${suffix}`;

                result.push(`using ${name} = ${pinName};`);
            }
        }

        for (let i = 0; i < this.mcu.usarts.length; i++) {
            let usart = this.mcu.usarts[i];
            let signals = usart.signals;
            let moduleSuffix = '_' + ("00" + i.toString(10)).substr(-2);
            for (let j = 0; j < signals.length; j++) {
                let s = signals[j];
                let name = s.group.toLowerCase() + moduleSuffix;
                let portName = s.port.name;
                let prefix = portName.replace(/port_(.)/, "p$1");
                let suffix = ("00" + s.offset.toString(10)).substr(-2);
                let pinName = `${prefix}_${suffix}`;

                result.push(`using ${name} = ${pinName};`);
            }
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
            `#include <zoal/arch/avr/atmega/adc.hpp>`,
            `#include <zoal/arch/avr/atmega/cfg.hpp>`,
            `#include <zoal/arch/avr/atmega/irq.hpp>`,
            `#include <zoal/arch/avr/atmega/spi.hpp>`,
            `#include <zoal/arch/avr/atmega/i2c.hpp>`,
            `#include <zoal/arch/avr/atmega/metadata.hpp>`,
            `#include <zoal/arch/avr/atmega/mux.hpp>`,
            `#include <zoal/arch/avr/atmega/usart.hpp>`,
            `#include <zoal/arch/avr/port.hpp>`,
            `#include <zoal/arch/avr/atmega/timer16.hpp>`,
            `#include <zoal/arch/avr/atmega/timer8.hpp>`,
            `#include <zoal/arch/power.hpp>`,
            `#include <zoal/arch/enable.hpp>`,
            `#include <zoal/gpio/api.hpp>`,
            `#include <zoal/gpio/pin.hpp>`,
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
            `    `,
            this.buildPortList().join('\n'),
            ``,
            this.buildTimerList().join('\n'),
            ``,
            this.buildSPIs().join('\n'),
            ``,
            this.buildI2Cs().join('\n'),
            ``,
            this.buildUSARTList().join('\n'),
            ``,
            this.buildADCList().join('\n'),
            ``,
            this.buildPinList().join('\n'),
            ``,
            this.buildPinAliases().join('\n'),
            ``,
            this.buildPortChain().join('\n'),
            '    using mux = ::zoal::arch::avr::atmega::mux<self_type>;',
            '    using cfg = ::zoal::arch::avr::atmega::cfg<self_type>;',
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
            '    using zoal::ct::integral_constant;',
            `    using ${sign_name} = ${sign};`,
            '',
            this.buildTimersMetadata(sign_name).join('\n'),
            '',
            this.buildUSARTSsMetadata(sign_name).join('\n'),
            '',
            this.buildSPIsMetadata(sign_name).join('\n'),
            '',
            this.buildI2CsMetadata(sign_name).join('\n'),
            '',
            this.buildADCsMetadata(sign_name).join('\n'),
            `}}`,
            '',
            `#endif`,
            ``
        ].join('\n');
    }
}

module.exports = ATmega;
