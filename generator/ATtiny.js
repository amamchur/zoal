const fs = require('fs');
const xml2js = require('xml2js');
const Avr = require('./Avr');
let exec = require('child_process').exec;
let path = require('path');

class ATtiny extends Avr {

}

module.exports = ATtiny;