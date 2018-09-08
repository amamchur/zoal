const fs = require('fs');
const xml2js = require('xml2js');
const program = require('commander');
const includesMap = {
    megaAVR: [
        '#include <zoal/arch/avr/atmega/mux.hpp>',
        '#include <zoal/arch/avr/atmega/cfg.hpp>',
        '#include <zoal/arch/avr/port.hpp>',
        '#include <zoal/mcu/base_mcu.hpp">',
        '#include <zoal/gpio/pin.hpp>',
        '#include <zoal/gpio/base_api.hpp>',
        '#include <zoal/gpio/port_link.hpp>'
    ]
};

program
    .version('0.0.0')
    .option('-f, --family [family]', 'MCU family', /^(atmega|attiny)$/)
    .option('-m, --metadata [metadata]', 'MCU metadata file')
    .option('-o, --out [out]', 'Output file')
    .parse(process.argv);
console.log('  - %s', program.family);
console.log('  - %s', program.metadata);
console.log('  - %s', program.out);

let buffer = '';
let mcu = {
    ports: [],
    portsMap: {}
};

function beginHeaderGuard(name) {
    name = name.toUpperCase();
    buffer += `#ifndef ZOAL_MCU_${name}_HPP\n`;
    buffer += `#ifndef ZOAL_MCU_${name}_HPP\n`;
}

function endHeaderGuard() {
    buffer += `#endif\n\n`;
}

function placeIncludes(family) {
    console.log(family);
    buffer += '\n';
    buffer += ['#include <stdint.h>'].concat(includesMap[family] || {}).join('\n');
    buffer += '\n\n';
}

function placeAliases() {
    buffer += 'template<uintptr_t Address, uint8_t PinMask>\n';
    buffer += 'using port = typename zoal::arch::avr::port<Address, PinMask>;\n\n';
}

function placePort(name, address, mask) {
    let dec = parseInt(address); // your number
    let hex = '0x' + ("0000" + dec.toString(16)).substr(-4);
    mask = '0x' + ("00" + mask.toString(16)).substr(-2);

    buffer += `using ${name} = port<${hex}, ${mask}>;\n`;
}

function getPortModule(modules) {
    let portModule = null;
    for (let i = 0; i < modules.length; i++) {
        let m = modules[i];
        if (m.$.name === 'PORT') {
            portModule = m;
            break;
        }
    }

    return portModule;
}

function placePorts(modules) {
    let portModule = getPortModule(modules);

    function minRegOffset(registers) {
        let array = [].concat(registers);
        array.sort(function (a, b) {
            let ah = parseInt(a.$.offset, 16);
            let bh = parseInt(b.$.offset, 16);
            return ah - bh;
        });

        return array[0].$.offset;
    }

    let ports = portModule['register-group'];
    for (let i = 0; i < ports.length; i++) {
        let port = ports[i];
        let mask = parseInt(port.register[0].$.mask, 16); //port
        let name = port.$.name.toLowerCase();
        let address = minRegOffset(port.register);
        let shortName = name.replace(/port(.)/, "$1");
        name = name.replace(/port(.)/, "port_$1");
        placePort(name, address, mask);

        let obj = {
            name: name,
            sn: shortName
        };

        mcu.ports.push(obj);
        mcu.portsMap[obj.sn] = obj;
    }
}

function placePins(modules) {
    let portModule = getPortModule(modules);
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
                let pinName = `${prefix}${suffix}`;
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

    // console.log(mcu.portsMap['a'].pins);
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
    placePins(root.modules[0].module);
    placeAPI();
    endClass();
    endNamespace();
    endHeaderGuard(device.$.name);
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