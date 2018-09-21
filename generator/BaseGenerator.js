
class BaseGenerator {
    constructor(file) {
        this.file = file;
        this.buffer = '';
        this.mcu = {};
    }

    static toHex(value, padding) {
        return '0x' + ("00000000" + value.toString(16)).substr(-padding).toUpperCase();
    }
}

module.exports = BaseGenerator;
