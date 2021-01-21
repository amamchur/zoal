
class BaseGenerator {
    constructor(file) {
        this.file = file;
        this.buffer = '';
        this.mcu = {};
    }

    build_signature(name) {
        let array = [];
        for (let i = 0; i < name.length; i++) {
            array.push(`'${name[i]}'`)
        }
        let chars = array.join(',');
        return `zoal::ct::signature<${chars}>`;
    }

    static toHex(value, padding) {
        return '0x' + ("00000000" + value.toString(16)).substr(-padding).toUpperCase();
    }
}

module.exports = BaseGenerator;
