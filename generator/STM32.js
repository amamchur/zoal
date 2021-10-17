const fs = require('fs');
const xml2js = require('xml2js');
const BaseGenerator = require('./BaseGenerator');
const exec = require('child_process').exec;
const path = require('path');

let svdData = {};
let familyMap = {
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
            '#include <zoal/gpio/pin.hpp>',
            '#include <zoal/arch/cortex/stm32x/timer.hpp>',
            '#include <zoal/arch/cortex/stm32x/i2c.hpp>'
        ],
        classDeclaration: [
            ``,
            `template<uintptr_t Address, class Clock, uint32_t PinMask>`,
            `using port = typename ::zoal::arch::stm32x::port<Address, Clock, PinMask>;`,
            ``
        ]
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
            '#include <zoal/arch/cortex/stm32f1/i2c.hpp>'
        ],
        classDeclaration: [
            `using afio = ::zoal::arch::stm32f1::afio<0x40010000, clock_apb2<0x00000001>>;`,
            ``,
            `template<uintptr_t Address, class Clock, uint32_t PinMask>`,
            `using port = typename ::zoal::arch::stm32f1::port<Address, Clock, PinMask>;`,
            ``
        ]
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
            '#include <zoal/arch/cortex/stm32x/i2c.hpp>',
            '#include <zoal/arch/cortex/stm32x/timer.hpp>',
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
        ]
    },
    'stm32f4': {
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
            '#include <zoal/arch/cortex/stm32x/i2c.hpp>',
            '#include <zoal/arch/cortex/stm32x/timer.hpp>',
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
        ]
    }
};

class STM32 extends BaseGenerator {
    constructor(file, className, svdFile) {
        super(file);

        let dir = path.dirname(file);
        this.configDir = path.join(dir);
        this.className = className;
        this.svdFile = svdFile;
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

            uart: function (name, no, data) {
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

            i2c: function (name, no, data) {
                let address = STM32.toHex(data.address, 8);
                let n = ("00" + no.toString(10)).substr(-2);
                let m = STM32.toHex(data.busClockMask, 8);
                result.push(`using ${name}_${n} = ::zoal::arch::${ns}::i2c<${address}, clock_${data.bus}<${m}>>;`);
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

            let nn = name.match(/(\w+[a-z])(\d+)/);
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
                console.log('Builder not found', nn[1]);
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
        this.metadata = metadata;
        this.mcu.family = metadata.Mcu.$.Family.toLowerCase();
        Object.assign(familyMap[this.mcu.family], svdData);
        this.collectPins(metadata);

        let gpioCfg = this.getConfigFile(metadata, 'GPIO');
        this.gpioConfigFilePath = path.join(this.configDir, 'IP', 'GPIO-' + gpioCfg.$.Version + '_Modes.xml');
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

    normalizePeriph(periphs) {
        for (let k in periphs) {
            let periph = periphs[k];
            if (periph.$ && periph.$.derivedFrom) {
                let base = periphs[periph.$.derivedFrom];
                periph[k] = Object.assign({}, base, periph);
            }
        }
    }

    collectResetAndClockControl(rcc, array) {
        function findRegister(name) {
            let registers = rcc.registers[0].register;
            for (let i = 0; i < registers.length; i++) {
                let r = registers[i];
                let n = r.name[0];
                if (n === name) {
                    return r;
                }
            }

            return null;
        }

        let ahb_enr = findRegister('AHBENR');
        let ahb1_enr = findRegister('AHB1ENR');
        let ahb2_enr = findRegister('AHB2ENR');
        let apb1_enr = findRegister('APB1ENR');
        let apb2_enr = findRegister('APB2ENR');

        function findField(reg, name) {
            let fields = reg.fields[0].field;
            for (let i = 0; i < fields.length; i++) {
                let f = fields[i];
                let n = f.name[0];
                if (n === name) {
                    return f;
                }
            }

            return null;
        }

        function findFieldByDescription(reg, regexp) {
            let fields = reg.fields[0].field;
            for (let i = 0; i < fields.length; i++) {
                let f = fields[i];
                let d = f.description[0];
                if (d.match(regexp)) {
                    return f;
                }
            }

            return null;
        }

        function findBusByFieldName(registers, name) {
            for (let i = 0; i < registers.length; i++) {
                let r = registers[i];
                let f = findField(r, name);
                if (f != null) {
                    return {
                        register: r,
                        field: f
                    };
                }
            }
            return null;
        }

        function findBusByFieldDescription(registers, regexp) {
            for (let i = 0; i < registers.length; i++) {
                let r = registers[i];
                let f = findFieldByDescription(r, regexp);
                if (f != null) {
                    return {
                        register: r,
                        field: f
                    };
                }
            }
            return null;
        }

        let rccRegisters = [ahb_enr, ahb1_enr, ahb2_enr, apb1_enr, apb2_enr].filter((i) => {
            return !!i
        });
        for (let i = 0; i < array.length; i++) {
            let periphs = array[i];
            for (let k in periphs) {
                let periph = periphs[k];
                let periphName = periph.name[0];
                let fieldName = periphName + 'EN';

                let fieldObj = findBusByFieldName(rccRegisters, fieldName);
                let cfg = {bus: '???', address: parseInt(periph.baseAddress[0], 16), busClockMask: 0};
                periph.periphConfig = cfg;

                if (fieldObj == null) {
                    fieldObj = findBusByFieldDescription(rccRegisters, new RegExp(`.*${periphName}.*clock enable.*`));
                }
                if (fieldObj == null) {
                    fieldObj = findBusByFieldDescription(rccRegisters, new RegExp(`.*${periphName}.*clock enable.*`));
                }

                if (fieldObj == null) {
                    fieldName = fieldName.replace(/^GPIO/, 'IOP');
                    fieldObj = findBusByFieldName(rccRegisters, fieldName);
                }

                if (fieldObj == null) {
                    continue;
                }

                let reg = fieldObj.register;
                let field = fieldObj.field;
                cfg.busClockMask = 1 << (field.bitOffset - 0);
                switch (reg.name[0]) {
                    case 'AHBENR':
                        cfg.bus = 'ahb';
                        break;
                    case 'AHB1ENR':
                        cfg.bus = 'ahb1';
                        break;
                    case 'AHB2ENR':
                        cfg.bus = 'ahb2';
                        break;
                    case 'APB1ENR':
                        cfg.bus = 'apb1';
                        break;
                    case 'APB2ENR':
                        cfg.bus = 'apb2';
                        break;
                }
            }
        }
    }

    collectPeriphData(obj) {
        let peripherals = obj.device.peripherals[0].peripheral;
        let ports = {};
        let adcs = {};
        let timers = {};
        let i2cs = {};
        let usarts = {};
        let uarts = {};
        let rcc = null;
        let portRegexp = /GPIO([A-Z])/;
        let adcRegexp = /ADC([\d+])/;
        let timerRegexp = /TIM([\d+])/;
        let i2cRegexp = /I2C([\d+])/;
        let usartsRegexp = /USART([\d+])/;
        let uartsRegexp = /UART([\d+])/;

        for (let i = 0; i < peripherals.length; i++) {
            let p = peripherals[i];
            let name = p.name[0];
            if (name.match(portRegexp)) {
                ports[name] = p;
            } else if (name.match(adcRegexp)) {
                adcs[name] = p;
            } else if (name.match(timerRegexp)) {
                timers[name] = p;
            } else if (name.match(i2cRegexp)) {
                i2cs[name] = p;
            } else if (name.match(usartsRegexp)) {
                usarts[name] = p;
            } else if (name.match(uartsRegexp)) {
                uarts[name] = p;
            } else if (name === 'RCC') {
                rcc = p;
            }
        }

        this.normalizePeriph(adcs);
        this.normalizePeriph(ports);
        this.normalizePeriph(timers);
        this.normalizePeriph(i2cs);
        this.normalizePeriph(usarts);
        this.normalizePeriph(uarts);
        this.collectResetAndClockControl(rcc, [adcs, ports, timers, i2cs, usarts, uarts]);

        let array = [adcs, timers, i2cs, usarts, uarts]
        let mcuData = {periphs: {}, ports: {}};
        for (let i = 0; i < array.length; i++) {
            let periphs = array[i];
            for (let k in periphs) {
                let periph = periphs[k];
                let periphName = periph.name[0].toLowerCase();
                mcuData.periphs[periphName] = Object.assign({svdData: periph}, periph.periphConfig);
            }
        }

        for (let k in ports) {
            let port = ports[k];
            let name = port.name[0];
            let portRegexp = /GPIO([A-Z])/;
            let m = name.match(portRegexp)
            let s = m[1].toLowerCase();
            mcuData.ports[s] = Object.assign({svdData: port}, port.periphConfig);
        }

        svdData = mcuData;
    }

    generate() {
        let promise = new Promise((resolve, reject) => {
            fs.readFile(this.svdFile, (err, data) => {
                    if (err) {
                        reject();
                        return;
                    }

                    const parser = new xml2js.Parser();
                    parser.parseString(data, (err, result) => {
                        if (err) {
                            reject();
                            return;
                        }
                        try {
                            this.collectPeriphData(result);
                            resolve();
                        } catch (e) {
                            reject(e);
                        }
                    });
                }
            );
        });

        return promise.then(() => {
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
        });
    }

    buildPinRemapMetadataFromMcuXml(sign_name) {
        let pins = this.metadata.Mcu.Pin;
        let data = familyMap[this.mcu.family];
        let result = [];
        for (let i = 0; i < pins.length; i++) {
            let pin = pins[i];
            let pinName = pin.$.Name;
            let pm = pinName.toLowerCase().match(/p(\w)(\d+)/);
            if (!pm) {
                continue;
            }

            let portName = pm[1];
            let offset = pm[2] - 0;
            let port = data.ports[portName];
            let signals = pin.Signal;

            for (let j = 0; j < signals.length; j++) {
                let signal = signals[j];
                let signalName = signal.$.Name;
                let vm = signalName.toLowerCase().match(/^([a-z0-9]+\d+)_(\w+)$/);
                if (!vm) {
                    continue;
                }

                let periphName = vm[1];
                let periph = data.periphs[periphName];
                if (!periph) {
                    continue;
                }

                let sm = signalName.toLowerCase().match(/^(\w+)_(\w+)$/);
                if (!sm) {
                    continue;
                }

                if (!periph || !port || !sm) {
                    continue;
                }

                let portHex = STM32.toHex(port.address, 8);
                let periphHex = STM32.toHex(periph.address, 8);
                let offsetHex = STM32.toHex(offset, 2);
                let sgnl = sm[2];
                let q = {
                    key: `${periphHex}${portHex}${offsetHex}${sgnl}`,
                    declaration: [
                        `template<> // ${pinName} -> ${signalName}`,
                        `struct stm32_signal_map<${sign_name}, ${periphHex}, ${portHex}, ${offsetHex}, signal::${sgnl}> : zoal::ct::integral_constant<int, 0> {};`
                    ]
                };
                result.push(q);
            }
        }

        return result;
    }

    buildPinRemapMetadataFromGpioCfg(sign_name) {
        let file = this.gpioConfigFilePath;
        let buffer = fs.readFileSync(file);

        const parser = new xml2js.Parser();
        let xmlData;
        parser.parseString(buffer.toString(), (err, rst) => {
            xmlData = rst;
        });

        let data = familyMap[this.mcu.family];
        let pins = xmlData.IP.GPIO_Pin;
        let result = [];

        function processRemap(pin, s, offset, port) {
            let pinName = pin.$.Name;
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
            let periphHex = STM32.toHex(periph.address, 8);
            let offsetHex = STM32.toHex(offset, 2);
            let sgnl = sm[2];
            result.push({
                key: `${periphHex}${portHex}${offsetHex}${sgnl}`,
                declaration: [
                    `template<> // ${pinName} -> ${signalName}`,
                    `struct stm32_signal_map<${sign_name}, ${periphHex}, ${portHex}, ${offsetHex}, signal::${sgnl}> : zoal::ct::integral_constant<int, ${vm[2]}> {};`
                ]
            });
        }

        for (let i = 0; i < pins.length; i++) {
            let pin = pins[i];
            let pinName = pin.$.Name;
            let pm = pinName.toLowerCase().match(/p(\w)(\d+)/);
            if (!pm) {
                continue;
            }

            let portName = pm[1];
            let offset = pm[2] - 0;
            let port = data.ports[portName];
            let signals = pin.PinSignal || [];

            for (let j = 0; j < signals.length; j++) {
                let signal = signals[j];
                if (signal.RemapBlock) {
                    processRemap(pin, signal, offset, port);
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
                let signalName = signal.$.Name;
                let sm = signalName.toLowerCase().match(/^(\w+)_(\w+)$/);
                if (!periph || !port || !sm) {
                    continue;
                }

                let portHex = STM32.toHex(port.address, 8);
                let periphHex = STM32.toHex(periph.address, 8);
                let offsetHex = STM32.toHex(offset, 2);
                let sgnl = sm[2];
                let obj = {
                    key: `${periphHex}${portHex}${offsetHex}${sgnl}`,
                    declaration: [
                        `template<> // ${pinName} -> ${signalName}`,
                        `struct stm32_signal_map<${sign_name}, ${periphHex}, ${portHex}, ${offsetHex}, signal::${sm[2]}> : zoal::ct::integral_constant<int, ${vm[1]}> {};`
                    ]
                };
                result.push(obj);
            }
        }

        return result;
    }

    buildPinRemapMetadata(sign_name) {
        let gpio_md = this.buildPinRemapMetadataFromGpioCfg(sign_name);
        let xml_md = this.buildPinRemapMetadataFromMcuXml(sign_name);
        let remapping = [].concat(gpio_md);
        for (let i = 0; i < xml_md.length; i++) {
            let obj = xml_md[i];
            let qqq = remapping.find((item) => {
                return item.key === obj.key;
            });

            if (!qqq) {
                remapping.push(obj);
            }
        }

        remapping.sort((a, b) => {
            return a.key.localeCompare(b.key);
        });

        let result = [];
        for (let i = 0; i < remapping.length; i++) {
            result = result.concat(remapping[i].declaration);
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
            `using clock_ahb1 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_ahb1, Mask>;`,
            ``,
            `template<uint32_t Mask>`,
            `using clock_ahb2 = ::zoal::arch::stm32x::bus_clock<rcc, zoal::arch::bus::cortex_ahb2, Mask>;`,
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
            `#include <cstdint>`,
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
            this.buildPinRemapMetadata(sign_name).join('\n'),
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

module
    .exports = STM32;
