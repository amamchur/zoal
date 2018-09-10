const fs = require('fs');
const xml2js = require('xml2js');
const program = require('commander');
const includesMap = {
    megaAVR: [
        '#include <zoal/arch/avr/atmega/mux.hpp>',
        '#include <zoal/arch/avr/atmega/cfg.hpp>',
        '#include <zoal/arch/avr/port.hpp>',
        '#include <zoal/mcu/base_mcu.hpp>',
        '#include <zoal/gpio/pin.hpp>',
        '#include <zoal/arch/avr/adc.hpp>',
        '#include <zoal/arch/avr/atmega/usart.hpp>',
        '#include <zoal/arch/avr/timer16.hpp>',
        '#include <zoal/arch/avr/timer8.hpp>',
        '#include <zoal/gpio/base_api.hpp>',
        '#include <zoal/gpio/port_link.hpp>',
        '#include <zoal/arch/avr/atmega/irq.hpp>'
    ]
};

program
    .version('0.0.0')
    .option('-f, --family [family]', 'MCU family', /^(atmega|attiny)$/)
    .option('-m, --metadata [metadata]', 'MCU metadata file')
    .option('-o, --out [out]', 'Output file')
    .parse(process.argv);

let buffer = '';
let mcu = {
    ports: [],
    portsMap: {}
};

function beginHeaderGuard(name) {
    name = name.toUpperCase();
    buffer += `#ifndef ZOAL_MCU_${name}_HPP\n`;
    buffer += `#define ZOAL_MCU_${name}_HPP\n`;
}

function endHeaderGuard() {
    buffer += `#endif\n\n`;
}

function placeIncludes(family) {
    buffer += '\n';
    buffer += ['#include <stdint.h>'].concat(includesMap[family] || {}).join('\n');
    buffer += '\n\n';
}

function placeAliases() {
    buffer += 'template<uintptr_t Address, uint8_t PinMask>\n';
    buffer += 'using port = typename ::zoal::arch::avr::port<Address, PinMask>;\n\n';
}

function placePort(name, address, mask) {
    let hex = '0x' + ("0000" + address.toString(16)).substr(-4);
    mask = '0x' + ("00" + mask.toString(16)).substr(-2);

    buffer += `using ${name} = port<${hex}, ${mask}>;\n`;
}

function getModule(modules, regex) {
    let result = null;
    for (let i = 0; i < modules.length; i++) {
        let m = modules[i];
        if (m.$.name.match(regex)) {
            result = m;
            break;
        }
    }

    return result;
}

function getModules(modules, regex) {
    let results = [];
    for (let i = 0; i < modules.length; i++) {
        let m = modules[i];
        if (m.$.name.match(regex)) {
            results.push(m);
        }
    }

    return results;
}

function registerOffset(registers, regex) {
    for (let i = 0; i < registers.length; i++) {
        let reg = registers[i];
        if (reg.$.name.match(regex)) {
            return parseInt(reg.$.offset, 16);
        }
    }

    return null;
}

function placePorts(modules) {
    let portModule = getModule(modules, /PORT/);
    let ports = portModule['register-group'];
    for (let i = 0; i < ports.length; i++) {
        let port = ports[i];
        let mask = parseInt(port.register[0].$.mask, 16); //port
        let name = port.$.name.toLowerCase();
        let address = registerOffset(port.register, /PIN\w/);
        let shortName = name.replace(/port(.)/, "$1");
        name = name.replace(/port(.)/, "port_$1");
        placePort(name, address, mask);

        let obj = {
            name: name,
            address: address,
            sn: shortName
        };

        mcu.ports.push(obj);
        mcu.portsMap[obj.sn] = obj;
    }
}

function placePins(modules) {

    buffer += 'template<class Port, uint8_t Offset>\n';
    buffer += 'using pin = typename ::zoal::gpio::pin<Port, Offset>;\n\n';

    let portModule = getModule(modules, /PORT/);
    let ports = portModule['register-group'];
    for (let i = 0; i < ports.length; i++) {
        buffer += '\n';
        let port = ports[i];
        let name = port.$.name.toLowerCase();
        let mask = parseInt(port.register[0].$.mask, 16); //port
        let portName = name.replace(/port(.)/, "port_$1");
        let shortName = name.replace(/port(.)/, "$1");
        let prefix = name.replace(/port(.)/, "p$1");
        let portObj = mcu.portsMap[shortName];
        portObj.pins = [];

        let offset = 0;
        while ((1 << offset) < mask) {
            let exists = (mask & (1 << offset));
            if (exists) {
                let suffix = ("00" + offset.toString(16)).substr(-2);
                let pos = '0x' + offset.toString(16);
                let pinName = `${prefix}_${suffix}`;
                buffer += `using ${pinName} = pin<${portName}, ${pos}>;\n`;
                portObj.pins[offset] = {
                    name: pinName,
                    offset: offset
                };
            }

            offset++;
        }
    }

    buffer += '\n';
}

function placeTimers(modules) {
    buffer += '\n';

    let array = getModules(modules, /^TC(8|16)/);
    let timers = [];
    let moduleMap = {};
    mcu.timers = [];
    mcu.timersMap = {};
    for (let i = 0; i < array.length; i++) {
        let m = array[i];
        let g = m['register-group'];
        for (let j = 0; j < g.length; j++) {
            let t = g[j];
            moduleMap[t.$.name] = m;
        }
        timers = timers.concat(g);
    }

    for (let i = 0; i < timers.length; i++) {
        let t = timers[i];
        let name = t.$.name;
        let address = registerOffset(t.register, /TCCR\dA/);
        let tccrc = registerOffset(t.register, /TCCR\dC/);
        let n = name.replace(/TC(\d+)/, '$1');

        name = 'timer_' + ("0000" + n).substr(-2);
        let obj = {
            name: name,
            avrName: t.$.name,
            sn: n,
            type: tccrc ? 'timer16' : 'timer8',
            address: address
        };

        mcu.timers.push(obj);
        mcu.timersMap[obj.sn] = obj;
    }

    mcu.timers.sort(function (a, b) {
        return a.sn - b.sn;
    });

    for (let i = 0; i < mcu.timers.length; i++) {
        let t = mcu.timers[i];
        let hex = '0x' + ("0000" + t.address.toString(16)).substr(-4);
        let m = moduleMap[t.avrName];

        if (t.type === 'timer16') {
            buffer += `using ${t.name} = ::zoal::arch::avr::atmega::timer16<${hex}, ${t.sn}>;\n`;
        } else {
            let async = m.$.name.match(/ASYNC/i) != null;
            buffer += `using ${t.name} = ::zoal::arch::avr::atmega::timer8<${hex}, ${t.sn}, ${async}>;\n`;
        }
    }

    buffer += '\n';
}

function placeUSARTs(modules) {
    buffer += '\n';

    let array = getModules(modules, /^USART$/);
    let moduleMap = {};
    let usarts = [];
    mcu.usarts = [];
    mcu.usartsMap = {};
    for (let i = 0; i < array.length; i++) {
        let m = array[i];
        let g = m['register-group'];
        for (let j = 0; j < g.length; j++) {
            let t = g[j];
            moduleMap[t.$.name] = m;
        }
        usarts = usarts.concat(g);
    }

    for (let i = 0; i < usarts.length; i++) {
        let u = usarts[i];
        let name = u.$.name;
        let address = registerOffset(u.register, /UCSR\dA/);
        let n = name.replace(/USART(\d+)/, '$1');

        name = 'usart_' + ("0000" + n).substr(-2);
        let obj = {
            name: name,
            avrName: u.$.name,
            sn: n,
            address: address
        };

        mcu.usarts.push(obj);
        mcu.usartsMap[obj.sn] = obj;
    }

    for (let i = 0; i < mcu.usarts.length; i++) {
        let u = mcu.usarts[i];
        let hex = '0x' + ("0000" + u.address.toString(16)).substr(-4);
        buffer += `template<class Buffer>\n`;
        buffer += `using ${u.name} = typename ::zoal::arch::avr::usart<${hex}, ${u.sn}, Buffer>;\n\n`;
    }

    buffer += '\n';
}

function placeADCs(modules) {
    buffer += '\n';

    let module = getModules(modules, /^ADC/)[0];
    let adc = module['register-group'][0];
    let address = registerOffset(adc.register, /ADC/);
    let hex = '0x' + ("0000" + address.toString(16)).substr(-4);

    buffer += `using adc_00 = ::zoal::arch::avr::adc<::zoal::arch::avr::mcu_type::atmega, ${hex}, 0>;\n\n`;
    console.log(adc);
}

function placeAPI() {
    let portsNames = [];
    for (let i = 0; i < mcu.ports.length; i++) {
        let port = mcu.ports[i];
        portsNames.push(port.name);
    }

    buffer += '\n';
    buffer += 'using port_chain = typename ::zoal::gpio::chain_builder<\n';
    buffer += portsNames.join(',\n');
    buffer += '>::chain;\n';

    buffer += 'using api = ::zoal::gpio::base_api<port_chain>;\n';
    buffer += 'using mux = ::zoal::arch::avr::atmega::mux<api>;\n';
    buffer += 'using cfg = ::zoal::arch::avr::atmega::cfg<api, Frequency>;\n';
    buffer += 'using irq = ::zoal::arch::avr::atmega::irq;\n';
}

function beginNamespace() {
    buffer += 'namespace zoal { namespace mcu {\n';
}

function endNamespace() {
    buffer += '}}\n\n';
}

function beginClass(name) {
    name = name.toLowerCase();
    buffer += 'template<uint32_t Frequency>\n';
    buffer += `class ${name} : public base_mcu<Frequency, 1> {`;
    buffer += 'public:';
}

function endClass() {
    buffer += '};\n';
}

function printPorts(metadata) {
    let root = metadata['avr-tools-device-file'];
    let device = root.devices[0].device[0];

    beginHeaderGuard(device.$.name);
    placeIncludes(device.$.family);
    beginNamespace();
    beginClass(device.$.name);
    placeAliases();
    placePorts(root.modules[0].module);
    placeTimers(root.modules[0].module);
    placeUSARTs(root.modules[0].module);
    placeADCs(root.modules[0].module);
    placePins(root.modules[0].module);
    placeAPI();
    endClass();
    endNamespace();
    endHeaderGuard();
}

function write() {
    fs.writeFile(program.out, buffer, function (err) {
        if (err) {
            return console.log(err);
        }

        var exec = require('child_process').exec;
        exec(`clang-format -i ${program.out}`, function callback(error, stdout, stderr) {
        });
    });
}

var parser = new xml2js.Parser();
fs.readFile(program.metadata, function (err, data) {
    parser.parseString(data, function (err, result) {
        printPorts(result);
        write();
    });
});