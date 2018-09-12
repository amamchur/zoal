const program = require('commander');
const ATmega = require('./generator/ATmega');
const ATtiny = require('./generator/ATtiny');

program
    .version('0.0.0')
    .option('-f, --family [family]', 'MCU family', /^(atmega|attiny)$/)
    .option('-m, --metadata [metadata]', 'MCU metadata file')
    .option('-o, --out [out]', 'Output file')
    .parse(process.argv);

let generator = null;

if (program.family === 'atmega') {
    generator = new ATmega(program.metadata);
}

if (program.family === 'attiny') {
    generator = new ATtiny(program.metadata);
}

generator.generate(program.out);
