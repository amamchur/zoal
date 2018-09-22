const fs = require('fs');
const xml2js = require('xml2js');
const BaseGenerator = require('./BaseGenerator');
let exec = require('child_process').exec;
let path = require('path');

function makePortAHB(index) {
    return {
        bus: 'ahb',
        address: 0x48000000 + 0x0400 * index,
        busClockMask: 1 << (17 + index)
    }
}

function makePortAPB2(index) {
    return {
        bus: 'apb2',
        address: 0x40010800 + 0x0400 * index,
        busClockMask: 1 << (2 + index)
    }
}

const familyMap = {
    'stm32f3': {
        includes: [
            '#include <zoal/arch/cortex/nested_vectored_interrupt_controller.hpp>',
            '#include <zoal/arch/cortex/stm32f3/adc.hpp>',
            '#include <zoal/arch/cortex/stm32f3/adc_common_regs.hpp>',
            '#include <zoal/arch/cortex/stm32f3/general_purpose_timer.hpp>',
            '#include <zoal/arch/cortex/stm32f3/spi.hpp>',
            '#include <zoal/arch/cortex/stm32x/bus_clock.hpp>',
            '#include <zoal/arch/cortex/stm32x/cfg.hpp>',
            '#include <zoal/arch/cortex/stm32x/mux.hpp>',
            '#include <zoal/arch/cortex/stm32x/port.hpp>',
            '#include <zoal/arch/cortex/stm32x/reset_and_clock_control.hpp>',
            '#include <zoal/arch/cortex/stm32x/usart.hpp>',
            '#include <zoal/arch/enable.hpp>',
            '#include <zoal/arch/power.hpp>',
            '#include <zoal/gpio/api.hpp>'
        ],
        portDeclaration: [
            `template<uintptr_t Address, class Clock, uint32_t PinMask>`,
            `using port = typename ::zoal::arch::stm32x::port<Address, Clock, PinMask>;`,
            ``
        ],
        ports: {
            a: makePortAHB(0),
            b: makePortAHB(1),
            c: makePortAHB(2),
            d: makePortAHB(3),
            e: makePortAHB(4),
            f: makePortAHB(5),
        },
        periphs: {
            adc1: {bus: 'ahb', address: 0x50000000, busClockMask: 0x10000000},
            adc2: {bus: 'ahb', address: 0x50000100, busClockMask: 0x10000000},
            adc3: {bus: 'ahb', address: 0x50000400, busClockMask: 0x20000000},
            adc4: {bus: 'ahb', address: 0x50000500, busClockMask: 0x20000000},
            comp1: null,
            comp2: null,
            comp3: null,
            comp4: null,
            comp5: null,
            comp6: null,
            comp7: null,
            dac1: null,
            i2c1: null,
            i2c2: null,
            i2c3: null,
            i2s2: null,
            i2s3: null,
            opamp1: null,
            opamp2: null,
            opamp3: null,
            opamp4: null,
            rcc_osc32: null,
            spi1: null,
            spi2: null,
            spi3: null,
            tim1: null,
            tim15: null,
            tim16: null,
            tim17: null,
            tim2: {bus: 'apb1', address: 0x40000000, busClockMask: 0x000000001},
            tim3: {bus: 'apb1', address: 0x40000400, busClockMask: 0x000000002},
            tim4: {bus: 'apb1', address: 0x40000800, busClockMask: 0x000000004},
            tim8: null,
            tsc_g1: null,
            tsc_g2: null,
            tsc_g3: null,
            tsc_g4: null,
            tsc_g5: null,
            tsc_g6: null,
            uart4: null,
            uart5: null,
            usart1: {bus: 'apb2', address: 0x40013800, busClockMask: 0x00004000},
            usart2: {bus: 'apb1', address: 0x40004400, busClockMask: 0x00020000},
            usart3: {bus: 'apb1', address: 0x40004800, busClockMask: 0x00040000},
        }
    }
};

class STM32 extends BaseGenerator {
    constructor(file) {
        super(file);

        let dir = path.dirname(file);
        this.configDir = path.join(dir, 'config');
    }

    getConfigFile(metadata, name) {
        let array = metadata.Mcu.IP;
        for (let i = 0; i < array.length; i++) {
            let el = array[i];
            if (el.$.Name === name) {
                return el;
            }
        }

        return null;
    }

    collectPins(metadata) {
        let array = metadata.Mcu.Pin;
        let pinMap = {};
        let pins = [];
        let portMap = {};
        let periphs = {};

        for (let i = 0; i < array.length; i++) {
            let pin = array[i];
            let name = pin.$.Name.toLowerCase();
            let match = name.match(/^p(\w)(\d{1,2})/);
            if (!match) {
                continue;
            }

            let offset = match[2] - 0;
            let obj = {
                name: 'p' + match[1] + '_' + ("00" + offset.toString(10)).substr(-2),
                port: match[1],
                portName: 'port_' + match[1],
                offset: match[2] - 0
            };
            pinMap[name] = obj;
            pins.push(obj);

            let signals = [];
            for (let j = 0; j < pin.Signal.length; j++) {
                let signal = pin.Signal[j];
                let sm = signal.$.Name.match(/(\w+)_(\w+)/);
                if (!sm) {
                    continue;
                }

                let periph = sm[1];
                let connection = sm[2];
                let periphNo = periph.match(/(\d+)$/);
                if (!periphNo) {
                    continue;
                }

                periphs[periph] = 1;
                // console.log(periph, connection);
                // signals.push({});
            }

            let port = portMap[obj.port] || {
                name: obj.port,
                mask: 0
            };
            port.mask |= (1 << obj.offset);
            portMap[obj.port] = port;
        }

        this.mcu.periphs = periphs;
        this.mcu.pins = pins.sort((a, b) => {
            return a.name.localeCompare(b.name);
        });

        this.collectPorts(portMap);
    }

    buildPeriphs() {
        let periphs = this.mcu.periphs;
        let result = [];
        let builders = {
            usart: function (name, no, data) {
                let address = STM32.toHex(data.address, 8);
                let n = ("00" + no.toString(10)).substr(-2);
                let m = STM32.toHex(data.busClockMask, 8);
                result.push(`template<class Buffer>`);
                result.push(`using ${name}_${n} = typename ::zoal::arch::stm32x::usart<${address}, ${no}, Buffer, clock_${data.bus}<${m}>>;`);
                result.push(``);
            },

            adc: function (name, no, data) {
                let address = STM32.toHex(data.address, 8);
                let n = ("00" + no.toString(10)).substr(-2);
                let m = STM32.toHex(data.busClockMask, 8);
                result.push(`using ${name}_${n} = adc<${address}, ${no}, clock_${data.bus}<${m}>>;`);
            },

            tim: function (name, no, data) {
                let address = STM32.toHex(data.address, 8);
                let n = ("00" + no.toString(10)).substr(-2);
                let m = STM32.toHex(data.busClockMask, 8);
                result.push(`using ${name}er_${n} = zoal::arch::stm32f3::general_purpose_timer<${address}, ${no}, clock_${data.bus}<${m}>>;`);
            }
        };

        let array = Object.keys(periphs).sort();
        let data = familyMap[this.mcu.family];
        let lastGroup = null;
        for (let i = 0; i < array.length; i++) {
            let name = array[i];
            name = name.toLowerCase();

            let nn = name.match(/(\w+)(\d+)/);
            if (lastGroup !== nn[1]) {
                result.push(``);
                lastGroup = nn[1];
            }

            let pd = data.periphs[name];
            if (!pd) {
                continue;
            }

            let fn = builders[nn[1]];
            if (fn) {
                fn(nn[1], nn[2] - 0, pd);
            }
        }

        return result;
    }

    collectPorts(portMap) {
        let keys = Object.keys(portMap).sort();
        let data = familyMap[this.mcu.family];
        this.mcu.ports = [];
        this.mcu.portsMaps = {};
        for (let i = 0; i < keys.length; i++) {
            let key = keys[i];
            let portData = data.ports[key];
            let clockMast = STM32.toHex(portData.busClockMask, 8);
            let port = {
                name: 'port_' + key,
                address: portData.address,
                pinMask: portMap[key].mask,
                clock: `clock_${portData.bus}<${clockMast}>`,
            };
            this.mcu.ports.push(port);
        }
    }

    collectData(metadata) {
        let name = metadata.Mcu.$.RefName;
        let nameMatch = name.match(/(stm32\w+)(\w)\((\w)\-(\w)\)/i);
        name = nameMatch[1];

        let l = nameMatch[3].charCodeAt(0);
        let r = nameMatch[4].charCodeAt(0);

        for (let i = l; i <= r; i++) {
            name += '_' + nameMatch[2] + String.fromCharCode(i);
        }

        this.mcu.name = name;
        this.mcu.family = metadata.Mcu.$.Family.toLowerCase();
        this.collectPins(metadata);

        // let gpioCfg = this.getConfigFile(metadata, 'GPIO');
        // this.gpioConfigFilePath = path.join(this.configDir, gpioCfg.$.ConfigFile + '_Configs.xml');
        // console.log(this.gpioConfigFilePath);
    }

    buildPortList() {
        let data = familyMap[this.mcu.family];
        let result = [].concat(data.portDeclaration);
        for (let i = 0; i < this.mcu.ports.length; i++) {
            let port = this.mcu.ports[i];
            let hex = STM32.toHex(port.address, 8);
            let mask = STM32.toHex(port.pinMask, 4);
            result.push(`using ${port.name} = port<${hex}, ${port.clock}, ${mask}>;`);
        }

        return result;
    }

    buildPinList() {
        let data = familyMap[this.mcu.family];
        let result = [
            'template<class Port, uint8_t Offset>',
            'using pin = typename ::zoal::gpio::pin<Port, Offset>;',
            ``
        ];

        let prevPort = null;
        for (let i = 0; i < this.mcu.pins.length; i++) {
            let pin = this.mcu.pins[i];
            let pos = STM32.toHex(pin.offset, 2);
            if (prevPort !== pin.portName) {
                result.push('');
                prevPort = pin.portName;
            }
            result.push(`using ${pin.name} = pin<${pin.portName}, ${pos}>;`);
        }

        return result;
    }

    buildPortChain() {
        let portsNames = [];
        for (let i = 0; i < this.mcu.ports.length; i++) {
            let port = this.mcu.ports[i];
            portsNames.push(port.name);
        }

        return [
            '',
            'using ports = ::zoal::ct::type_list<',
            portsNames.join(',\n'),
            '>;'
        ];
    }

    generate() {
        return new Promise((resolve, reject) => {
            fs.readFile(this.file, (err, data) => {
                if (err) {
                    reject(err);
                }

                const parser = new xml2js.Parser();
                parser.parseString(data, (err, result) => {
                    try {
                        this.collectData(result);
                        this.buildClass();

                        resolve();
                    } catch (e) {
                        reject(e);
                    }
                });
            });
        })
    }

    buildClass() {
        let name = this.mcu.name;
        let nameUpper = name.toUpperCase();
        let nameLower = name.toLowerCase();

        let data = familyMap[this.mcu.family];

        this.buffer = [
            `/**`,
            ` * Autogenerated file. Do not modify manually.`,
            ` */`,
            `#ifndef ZOAL_MCU_${nameUpper}_HPP`,
            `#define ZOAL_MCU_${nameUpper}_HPP`,
            ``,
            `#include <stdint.h>`,
            data.includes.join('\n'),
            ``,
            `namespace zoal { namespace mcu {`,
            `    template<uint32_t HighSpeedExternalOscillator = 8000000, uint8_t PhaseLockedLoop = 9>`,
            `    class ${nameLower} {`,
            `    public:`,
            `        static constexpr auto hse = HighSpeedExternalOscillator;`,
            `        static constexpr auto pll = PhaseLockedLoop;`,
            `        static constexpr auto frequency = hse * pll;`,
            ``,
            `        using self_type = ${nameLower}<hse, pll>;`,
            `        using nvic = ::zoal::arch::stm32f1::nested_vectored_interrupt_controller<>;`,
            `        using rcc = ::zoal::arch::stm32x::reset_and_clock_control<>;`,
            ``,
            `    template<uint32_t Mask>`,
            `    using clock_ahb = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_ahb, Mask>;`,
            ``,
            `    template<uint32_t Mask>`,
            `    using clock_apb1 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_apb1, Mask>;`,
            ``,
            `    template<uint32_t Mask>`,
            `    using clock_apb2 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_apb2, Mask>;`,
            ``,
            `    template<uintptr_t Address, uint8_t N, class Clock>`,
            `    using adc = typename ::zoal::arch::stm32f3::adc<Address, N, Clock>;`,
            ``,
            this.buildPortList().join('\n'),
            ``,
            this.buildPeriphs().join('\n'),
            ``,
            this.buildPinList().join('\n'),
            ``,
            this.buildPortChain().join('\n'),
            `    using api = ::zoal::gpio::api<ports>;`,
            `    using mux = ::zoal::arch::stm32x::mux<self_type>;`,
            `    using cfg = ::zoal::arch::stm32x::cfg<self_type>;`,
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
            '',
            `}}`,
            '',
            `#endif`,
            ``
        ].join('\n');
    }

    writeFile(outFile) {
        fs.writeFile(outFile, this.buffer, function (err) {
            if (err) {
                return console.log(err);
            }

            let dir = path.dirname(outFile);

            exec(`clang-format -i ${outFile}`, {
                cwd: dir
            });
        });
    }
}

module.exports = STM32;
