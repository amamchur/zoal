const program = require('commander');
const path = require('path');
const ATmega = require('./generator/ATmega');
const ATtiny = require('./generator/ATtiny');
const STM32 = require('./generator/STM32');

// node index.js -f stm32 -m ~/STM32CubeMX/db/mcu/STM32F303V(B-C)Tx.xml -o stm32f303vctx.hpp

program
    .version('0.0.0')
    .option('-f, --family [family]', 'MCU family', /^(atmega|attiny|stm32)$/)
    .option('-m, --metadata [metadata]', 'MCU metadata file')
    .option('-o, --out [out]', 'Output file')
    .parse(process.argv);

if (!program.family || !program.metadata || !program.out) {
    console.log('Incorrect program arguments');
    process.exit(-1);
}

let obj = path.parse(program.out);
let generator = null;

if (program.family === 'atmega') {
    generator = new ATmega(program.metadata);
}

if (program.family === 'attiny') {
    generator = new ATtiny(program.metadata);
}

if (program.family === 'stm32') {
    generator = new STM32(program.metadata, obj.name);
}

if (generator) {
    generator.generate()
        .then(() => {
            generator.writeFile(program.out);
        })
        .catch((exc) => {
            console.log(exc);
        });
}
