const fs = require('fs');
const xml2js = require('xml2js');
const BaseGenerator = require('./BaseGenerator');
let exec = require('child_process').exec;
let path = require('path');

// #define PERIPH_BASE           ((uint32_t)0x40000000)
// #define APB1PERIPH_BASE       ((uint32_t)0x40000000)
// #define APB2PERIPH_BASE       (PERIPH_BASE + 0x10000)
// #define AHBPERIPH_BASE        (PERIPH_BASE + 0x20000)

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
    'stm32f0': {
        ns: 'stm32x',
        includes: [
            '#include <zoal/arch/cortex/stm32x/bus_clock.hpp>',
            '#include <zoal/arch/cortex/stm32x/cfg.hpp>',
            '#include <zoal/arch/cortex/stm32x/mux.hpp>',
            '#include <zoal/arch/cortex/stm32x/port.hpp>',
            '#include <zoal/arch/cortex/stm32x/rcc.hpp>',
            '#include <zoal/arch/cortex/stm32x/usart.hpp>',
            '#include <zoal/arch/cortex/stm32x/metadata.hpp>',
            '#include <zoal/arch/enable.hpp>',
            '#include <zoal/arch/power.hpp>',
            '#include <zoal/gpio/api.hpp>',
            '#include <zoal/gpio/pin.hpp>'
        ],
        classDeclaration: [
            ``,
            `template<uintptr_t Address, class Clock, uint32_t PinMask>`,
            `using port = typename ::zoal::arch::stm32x::port<Address, Clock, PinMask>;`,
            ``
        ],
        periphs: {
            usart1: {bus: 'apb2', address: 0x40013800, busClockMask: 0x00004000},
            usart2: {bus: 'apb1', address: 0x40004400, busClockMask: 0x00020000},
            usart3: {bus: 'apb1', address: 0x40004800, busClockMask: 0x00040000}
        },
        ports: {
            a: makePortAHB(0),
            b: makePortAHB(1),
            c: makePortAHB(2),
            d: makePortAHB(3),
            e: makePortAHB(4),
            f: makePortAHB(5),
        }
    },
    'stm32f1': {
        ns: 'stm32f1',
        includes: [
            '#include <zoal/arch/cortex/stm32x/bus_clock.hpp>',
            '#include <zoal/arch/cortex/stm32f1/afio.hpp>',
            '#include <zoal/arch/cortex/stm32f1/cfg.hpp>',
            '#include <zoal/arch/cortex/stm32f1/mux.hpp>',
            '#include <zoal/arch/cortex/stm32x/rcc.hpp>',
            '#include <zoal/arch/cortex/stm32x/metadata.hpp>',
            '#include <zoal/arch/enable.hpp>',
            '#include <zoal/arch/power.hpp>',
            '#include <zoal/gpio/api.hpp>',
            '#include <zoal/arch/cortex/stm32f1/adc.hpp>',
            '#include <zoal/arch/cortex/stm32f1/port.hpp>',
            '#include <zoal/arch/cortex/stm32f1/usart.hpp>',
            '#include <zoal/arch/cortex/stm32f1/timer.hpp>',
            '#include <zoal/arch/cortex/stm32f1/metadata.hpp>',
        ],
        classDeclaration: [
            `using afio = ::zoal::arch::stm32f1::afio<0x40010000, clock_apb2<0x00000001>>;`,
            ``,
            `template<uintptr_t Address, class Clock, uint32_t PinMask>`,
            `using port = typename ::zoal::arch::stm32f1::port<Address, Clock, PinMask>;`,
            ``
        ],
        ports: {
            a: makePortAPB2(0),
            b: makePortAPB2(1),
            c: makePortAPB2(2),
            d: makePortAPB2(3),
            e: makePortAPB2(4),
            f: makePortAPB2(5),
        },
        periphs: {
            usart1: {bus: 'apb2', address: 0x40013800, busClockMask: 0x00004000},
            usart2: {bus: 'apb1', address: 0x40004400, busClockMask: 0x00020000},
            usart3: {bus: 'apb1', address: 0x40004800, busClockMask: 0x00040000},
            adc1: {bus: 'apb2', address: 0x40012400, busClockMask: 0x00001000},
            adc2: {bus: 'apb2', address: 0x40012800, busClockMask: 0x00002000},

            tim1: {bus: 'apb2', address: 0x40012C00, busClockMask: 0x00000800},
            tim2: {bus: 'apb1', address: 0x40000000, busClockMask: 0x00000001},
            tim3: {bus: 'apb1', address: 0x40000400, busClockMask: 0x00000002},
            tim4: {bus: 'apb1', address: 0x40000800, busClockMask: 0x00000004},
            tim5: {bus: 'apb1', address: 0x40000C00, busClockMask: 0x00000008},
            tim6: {bus: 'apb1', address: 0x40001000, busClockMask: 0x00000010},
            tim7: {bus: 'apb1', address: 0x40001400, busClockMask: 0x00000020},
            tim8: {bus: 'apb2', address: 0x40013400, busClockMask: 0x00002000},
            tim9: {bus: 'apb2', address: 0x40014C00, busClockMask: 0x00080000},
            tim10: {bus: 'apb2', address: 0x40015000, busClockMask: 0x00100000},
            tim11: {bus: 'apb2', address: 0x40015400, busClockMask: 0x00200000},
            tim12: {bus: 'apb1', address: 0x40001800, busClockMask: 0x00000040},
            tim13: {bus: 'apb1', address: 0x40001C00, busClockMask: 0x00000080},
            tim14: {bus: 'apb1', address: 0x40002000, busClockMask: 0x00000100},
        }
    },
    'stm32f3': {
        ns: 'stm32x',
        includes: [
            '#include <zoal/arch/cortex/stm32f3/adc.hpp>',
            '#include <zoal/arch/cortex/stm32f3/adc_common_regs.hpp>',
            '#include <zoal/arch/cortex/stm32f3/general_purpose_timer.hpp>',
            '#include <zoal/arch/cortex/stm32f3/spi.hpp>',
            '#include <zoal/arch/cortex/stm32x/bus_clock.hpp>',
            '#include <zoal/arch/cortex/stm32x/cfg.hpp>',
            '#include <zoal/arch/cortex/stm32x/mux.hpp>',
            '#include <zoal/arch/cortex/stm32x/port.hpp>',
            '#include <zoal/arch/cortex/stm32x/rcc.hpp>',
            '#include <zoal/arch/cortex/stm32x/usart.hpp>',
            '#include <zoal/arch/cortex/stm32x/metadata.hpp>',
            '#include <zoal/arch/enable.hpp>',
            '#include <zoal/arch/power.hpp>',
            '#include <zoal/gpio/api.hpp>',
            '#include <zoal/gpio/pin.hpp>'
        ],
        classDeclaration: [
            `template<uintptr_t Address, class Clock>`,
            `using adc = typename ::zoal::arch::stm32x::adc<Address, Clock>;`,
            ``,
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
    constructor(file, className) {
        super(file);

        let dir = path.dirname(file);
        this.configDir = path.join(dir);
        this.className = className;
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

                let p = periphs[periph] || {signals: []};
                p.signals.push({
                    pin: pin,
                    pinData: obj,
                    signal: signal
                });

                periphs[periph] = p;
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
        let data = familyMap[this.mcu.family];
        let ns = data.ns;
        let builders = {
            usart: function (name, no, data) {
                let address = STM32.toHex(data.address, 8);
                let n = ("00" + no.toString(10)).substr(-2);
                let m = STM32.toHex(data.busClockMask, 8);
                result.push(`using ${name}_${n} = typename ::zoal::arch::${ns}::usart<${address}, clock_${data.bus}<${m}>>;`);
            },

            adc: function (name, no, data) {
                let address = STM32.toHex(data.address, 8);
                let n = ("00" + no.toString(10)).substr(-2);
                let m = STM32.toHex(data.busClockMask, 8);
                result.push(`using ${name}_${n} = ::zoal::arch::${ns}::adc<${address}, clock_${data.bus}<${m}>>;`);
            },

            tim: function (name, no, data) {
                let address = STM32.toHex(data.address, 8);
                let n = ("00" + no.toString(10)).substr(-2);
                let m = STM32.toHex(data.busClockMask, 8);
                result.push(`using ${name}er_${n} = zoal::arch::${ns}::timer<${address}, clock_${data.bus}<${m}>>;`);
            }
        };

        let array = Object.keys(periphs).sort();
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
            } else {
                console.log(nn[1]);
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
        this.mcu.family = metadata.Mcu.$.Family.toLowerCase();
        this.collectPins(metadata);

        let gpioCfg = this.getConfigFile(metadata, 'GPIO');
        this.gpioConfigFilePath = path.join(this.configDir, 'IP', 'GPIO-' + gpioCfg.$.Version + '_Modes.xml');
        console.log(this.gpioConfigFilePath);

        // GPIO-STM32F303E_gpio_v1_0_Modes.xml
        // GPIO-STM32F303E_gpio_v1_0_Modes
    }

    buildPortList() {
        let result = [];
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

    buildUSARTSsMetadata(sign_name) {
        let result = [];
        let periphs = this.mcu.periphs;
        let keys = Object.keys(periphs);

        let file = this.gpioConfigFilePath;
        let buffer = fs.readFileSync(file);

        const parser = new xml2js.Parser();
        let xmlData;
        parser.parseString(buffer.toString(), (err, rst) => {
            xmlData = rst;
        });

        let data = familyMap[this.mcu.family];
        let pins = xmlData.IP.GPIO_Pin;
        let exp = [];
        let z = [];

        function processRemap(pin, s, offset, port) {
            let signalName = s.$.Name;
            let sm = signalName.toLowerCase().match(/^(\w+)_(\w+)$/);
            let name = s.RemapBlock[0].$.Name;
            let vm = name.toLowerCase().match(/^(\w+)_remap(\d+)$/);
            let periphName = vm[1];
            let periph = data.periphs[periphName];
            if (!periph) {
                return;
            }

            let portHex = STM32.toHex(port.address, 8);
            let usartHex = STM32.toHex(periph.address, 8);
            let offsetHex = STM32.toHex(offset, 2);

            z.push({
                sortKey: `${usartHex}${portHex}${offsetHex}`,
                declaration: [
                    `template<> // ${name} -> ${periphName}`,
                    `struct stm32_remap<${sign_name}, ${usartHex}, ${portHex}, ${offsetHex}, signal::${sm[2]}> : zoal::ct::integral_constant<int, ${vm[2]}> {};`
                ]
            });
        }

        for (let i = 0; i < pins.length; i++) {
            let pin = pins[i];
            let pinName = pin.$.Name;
            let pm = pinName.toLowerCase().match(/p(\w)(\d+)/);
            let portName = pm[1];
            let offset = pm[2] - 0;
            let port = data.ports[portName];
            let signals = pin.PinSignal || [];

            for (let j = 0; j < signals.length; j++) {
                let signal = signals[j];
                let signalName = signal.$.Name;
                if (signal.RemapBlock) {
                    processRemap(pin, signal, offset, port);
                    continue;
                }

                if (!signal.SpecificParameter) {
                    continue;
                }

                let value = signal.SpecificParameter[0].PossibleValue[0];
                let vm = value.toLowerCase().match(/^gpio_af(\d+)_(\w+)$/);
                if (!vm) {
                    continue;
                }

                let periphName = vm[2];
                let periph = data.periphs[periphName];

                let sm = signalName.toLowerCase().match(/^(\w+)_(\w+)$/);
                if (!periph || !port || !sm) {
                    continue;
                }

                let portHex = STM32.toHex(port.address, 8);
                let usartHex = STM32.toHex(periph.address, 8);
                let offsetHex = STM32.toHex(offset, 2);
                let q = {
                    sortKey: `${signalName}${portHex}${offsetHex}`,
                    declaration: [
                        `template<> // ${pinName} -> ${signalName}`,
                        `struct stm32_af<${sign_name}, ${usartHex}, ${portHex}, ${offsetHex}, signal::${sm[2]}> : zoal::ct::integral_constant<int, ${vm[1]}> {};`
                    ]
                };
                z.push(q);
            }
        }

        z.sort((a, b) => {
            return a.sortKey.localeCompare(b.sortKey);
        });

        for (let i = 0; i < z.length; i++) {
            result = result.concat(z[i].declaration);
        }

        return result;
    }

    buildClocks() {
        return [
            `using rcc = ::zoal::arch::stm32x::rcc<>;`,
            ``,
            `template<uint32_t Mask>`,
            `using clock_ahb = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_ahb, Mask>;`,
            ``,
            `template<uint32_t Mask>`,
            `using clock_apb1 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_apb1, Mask>;`,
            ``,
            `template<uint32_t Mask>`,
            `using clock_apb2 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_apb2, Mask>;`,
        ];
    }

    buildFamilySpec() {
        let data = familyMap[this.mcu.family];
        return data.classDeclaration;
    }

    buildClass() {
        let nameUpper = this.className.toUpperCase();
        let nameLower = this.className.toLowerCase();
        let sign = this.build_signature(nameLower);
        let sign_name = `${nameLower}_sign`;
        let data = familyMap[this.mcu.family];
        let ns = data.ns;

        this.buffer = [
            `/**`,
            ` * Autogenerated file. Do not modify manually.`,
            ` */`,
            `#ifndef ZOAL_MCU_${nameUpper}_HPP`,
            `#define ZOAL_MCU_${nameUpper}_HPP`,
            ``,
            `#include <stdint.h>`,
            data.includes.join('\n'),
            `#include <zoal/ct/signature.hpp>`,
            ``,
            `namespace zoal { namespace mcu {`,
            `    class ${nameLower} {`,
            `    public:`,
            `        using self_type = ${nameLower};`,
            `        using signature = ${sign};`,
            ``,
            this.buildClocks().join('\n'),
            ``,
            this.buildFamilySpec().join('\n'),
            ``,
            this.buildPortList().join('\n'),
            ``,
            this.buildPeriphs().join('\n'),
            ``,
            this.buildPinList().join('\n'),
            ``,
            this.buildPortChain().join('\n'),
            `    using api = ::zoal::gpio::api;`,
            `    using mux = ::zoal::arch::${ns}::mux<self_type>;`,
            `    using cfg = ::zoal::arch::${ns}::cfg<self_type>;`,
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
            this.buildUSARTSsMetadata(sign_name).join('\n'),
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

            exec(`clang-format --style=file -i ${outFile}`, {});
        });
    }
}

module.exports = STM32;
