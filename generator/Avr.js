const fs = require('fs');
const xml2js = require('xml2js');
let exec = require('child_process').exec;
let path = require('path');

class Avr {
    constructor(file) {
        this.file = file;
        this.buffer = '';
        this.mcu = {};
    }

    static toHex(value, padding) {
        return '0x' + ("00000000" + value.toString(16)).substr(-padding).toUpperCase();
    }

    static getModule(modules, regex) {
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

    static getByName(modules, regex) {
        let results = [];
        for (let i = 0; i < modules.length; i++) {
            let m = modules[i];
            if (m.$.name.match(regex)) {
                results.push(m);
            }
        }

        return results;
    }

    static getByInstanceName(modules, regex) {
        let results = [];
        for (let i = 0; i < modules.length; i++) {
            let m = modules[i];
            for (let j = 0; j < m.instance.length; j++) {
                let name = m.instance[j].$.name;
                if (name.match(regex)) {
                    results.push(m);
                }
            }
        }

        return results;
    }

    static registerOffset(registers, regex) {
        for (let i = 0; i < registers.length; i++) {
            let reg = registers[i];
            if (reg.$.name.match(regex)) {
                return parseInt(reg.$.offset, 16);
            }
        }

        return null;
    }

    collectPorts(modules) {
        let portModule = Avr.getModule(modules, /PORT/);
        let ports = portModule['register-group'];
        this.mcu.ports = [];
        this.mcu.portsMap = {};
        for (let i = 0; i < ports.length; i++) {
            let port = ports[i];
            let mask = parseInt(port.register[0].$.mask, 16);
            let name = port.$.name.toLowerCase();
            let address = Avr.registerOffset(port.register, /PIN\w/);
            let shortName = name.replace(/port(.)/, "$1");
            name = name.replace(/port(.)/, "port_$1");

            let obj = {
                name: name,
                address: address,
                sn: shortName,
                pinMask: mask
            };

            this.mcu.ports.push(obj);
            this.mcu.portsMap[obj.sn] = obj;
        }
    }

    collectTimers(root, modules) {
        let timers_group = Avr.getByName(modules, /^TC(8|16)/);
        let timers = [];
        let moduleMap = {};
        this.mcu.timers = [];
        this.mcu.timersMap = {};
        for (let i = 0; i < timers_group.length; i++) {
            let m = timers_group[i];
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
            let address = Avr.registerOffset(t.register, /TCCR\dA/);
            let tccrc = Avr.registerOffset(t.register, /TCCR\dC/);
            let n = name.replace(/TC(\d+)/, '$1');

            let regex = new RegExp(t.$.name);
            let periph = Avr.getByInstanceName(root.devices[0].device[0].peripherals[0].module, regex)[0];
            let array = periph.instance[0].signals[0].signal;
            let signals = [];

            for (let j = 0; j < array.length; j++) {
                let s = array[j];
                let g = s.$.group;
                let pad = s.$.pad.toLowerCase();
                let port = pad.replace(/p(\w)\d/, '$1');
                let offset = pad.replace(/p\w(\d)/, '$1');
                if (g === 'OCA' || g === 'OCB') {
                    signals.push({
                        channel: g === 'OCA' ? 0 : 1,
                        pad: pad,
                        port: this.mcu.portsMap[port],
                        offset: offset - 0
                    });
                }
            }

            name = 'timer_' + ("0000" + n).substr(-2);
            let obj = {
                name: name,
                avrName: t.$.name,
                sn: n,
                type: tccrc ? 'timer16' : 'timer8',
                address: address,
                signals: signals,
                module: moduleMap[t.$.name]
            };

            this.mcu.timers.push(obj);
            this.mcu.timersMap[obj.sn] = obj;
        }

        this.mcu.timers.sort(function (a, b) {
            return a.sn - b.sn;
        });
    }

    collectUSARTs(root) {
        let array = Avr.getByName(root.modules[0].module, /^USART$/);
        let moduleMap = {};
        let usarts = [];
        this.mcu.usarts = [];
        this.mcu.usartsMap = {};
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
            let address = Avr.registerOffset(u.register, /UCSR\dA/);
            let n = name.replace(/USART(\d+)/, '$1');
            let periph = Avr.getByName(root.devices[0].device[0].peripherals[0].module, /^USART/)[0];
            let signals = periph.instance[0].signals[0].signal;

            let array = [];
            for (let j = 0; j < signals.length; j++) {
                let signal = signals[j];
                let pad = signal.$.pad.toLowerCase();
                let port = pad.replace(/p(\w)\d/, '$1');
                let offset = pad.replace(/p\w(\d)/, '$1');
                array.push({
                    group: signal.$.group,
                    pad: pad,
                    port: this.mcu.portsMap[port],
                    offset: offset - 0
                });
            }

            name = 'usart_' + ("0000" + n).substr(-2);
            let obj = {
                name: name,
                avrName: u.$.name,
                sn: n,
                address: address,
                signals: array
            };

            this.mcu.usarts.push(obj);
            this.mcu.usartsMap[obj.sn] = obj;
        }
    }

    collectADCs(root) {
        let module = Avr.getByName(root.modules[0].module, /^ADC/)[0];
        let periph = Avr.getByName(root.devices[0].device[0].peripherals[0].module, /^ADC/)[0];
        let signals = periph.instance[0].signals[0].signal;
        let adc = module['register-group'][0];
        let address = Avr.registerOffset(adc.register, /^ADC$/);

        let array = [];
        for (let i = 0; i < signals.length; i++) {
            let signal = signals[i];
            let pad = signal.$.pad.toLowerCase();
            let port = pad.replace(/p(\w)\d/, '$1');
            let offset = pad.replace(/p\w(\d)/, '$1');
            array.push({
                channel: signal.$.index - 0,
                pad: pad,
                port: this.mcu.portsMap[port],
                offset: offset - 0
            });
        }

        this.mcu.adcs = [{
            name: 'adc_00',
            sn: '0',
            address: address,
            signals: array
        }];
    }

    collectData(metadata) {
        let root = metadata['avr-tools-device-file'];
        let modules = root.modules[0].module;

        this.device = root.devices[0].device[0];
        this.collectPorts(modules);
        this.collectTimers(root, modules);
        this.collectUSARTs(root);
        this.collectADCs(root);
    }

    buildPortList() {
        let result = [];
        for (let i = 0; i < this.mcu.ports.length; i++) {
            let port = this.mcu.ports[i];
            let hex = Avr.toHex(port.address, 4);
            let mask = Avr.toHex(port.pinMask, 2);
            result.push(`using ${port.name} = port<${hex}, ${mask}>;`);
        }
        return result;
    }

    buildPinList() {
        let result = [
            'template<class Port, uint8_t Offset>',
            'using pin = typename ::zoal::gpio::pin<Port, Offset>;',
            ``
        ];

        for (let i = 0; i < this.mcu.ports.length; i++) {
            let port = this.mcu.ports[i];
            let name = port.name;
            let mask = port.pinMask;
            let prefix = name.replace(/port_(.)/, "p$1");
            let offset = 0;

            while ((1 << offset) < mask) {
                let exists = (mask & (1 << offset));
                if (exists) {
                    let suffix = ("00" + offset.toString(16)).substr(-2);
                    let pos = offset.toString(16);
                    let pinName = `${prefix}_${suffix}`;
                    result.push(`using ${pinName} = pin<${port.name}, ${pos}>;`);
                }

                offset++;
            }

            result.push(``);
        }

        return result;
    }

    generate(outFile) {
        const parser = new xml2js.Parser();

        fs.readFile(this.file, (err, data) => {
            parser.parseString(data, (err, result) => {
                this.collectData(result);
                this.buildClass();
                this.writeFile(outFile)
            });
        });
    }

    buildClass() {
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

module.exports = Avr;
