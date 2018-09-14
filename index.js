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

generator.generate()
    .then(() => {
        generator.writeFile(program.out);
    })
    .catch((exc) => {
        console.log(exc);
    });

// const fs = require('fs');
// let dir = 'C:\\Program Files (x86)\\Atmel\\Studio\\7.0\\packs\\atmel\\ATtiny_DFP\\1.3.172\\atdf';
// let path = require('path');
// let files = [];
// let badList = [];
//
// function printKey(obj) {
//     let a = [];
//     for (let i = 0; i < obj.array.length; i++) {
//         let r = obj.array[i];
//         a.push(`${r.avrName}[${r.offset}]`);
//     }
//
//     console.log(a.join(';'));
// }
//
// function processFile() {
//     if (files.length === 0) {
//         console.log(badList);
//         return;
//     }
//
//     let file = files.shift();
//
//     console.log('Processing: ', file);
//     let generator = new ATtiny(file);
//     generator.generate()
//         .then(() => {
//             printKey(generator.mcu.modules['ADC']);
//             process.nextTick(processFile);
//         })
//         .catch((exc) => {
//             badList.push(file);
//             console.log(exc);
//             process.nextTick(processFile);
//         });
// }
//
// fs.readdir(dir, function (err, items) {
//     for (let i = 0; i < items.length; i++) {
//         let file = items[i];
//         let filePath = path.join(dir, file);
//         files.push(filePath);
//     }
//
//     processFile();
// });
