#!/usr/bin/env bash
node index.js -f atmega -m /home/andrii/Documents/src/zoal-mcu-metadata/ATmega_DFP/1.2.209/atdf/ATmega328P.atdf -o include/zoal/mcu/atmega328p.hpp
node index.js -f atmega -m /home/andrii/Documents/src/zoal-mcu-metadata/ATmega_DFP/1.2.209/atdf/ATmega32U4.atdf -o include/zoal/mcu/atmega32u4.hpp
node index.js -f atmega -m /home/andrii/Documents/src/zoal-mcu-metadata/ATmega_DFP/1.2.209/atdf/ATmega2560.atdf -o include/zoal/mcu/atmega2560.hpp