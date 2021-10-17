#!/usr/bin/env bash
# ATmega
node index.js -f atmega -m ../zoal-mcu-metadata/ATmega_DFP/1.2.209/atdf/ATmega328P.atdf -o include/zoal/mcu/atmega328p.hpp
node index.js -f atmega -m ../zoal-mcu-metadata/ATmega_DFP/1.2.209/atdf/ATmega32U4.atdf -o include/zoal/mcu/atmega32u4.hpp
node index.js -f atmega -m ../zoal-mcu-metadata/ATmega_DFP/1.2.209/atdf/ATmega2560.atdf -o include/zoal/mcu/atmega2560.hpp
# ATtiny
node index.js -f attiny -m ../zoal-mcu-metadata/ATtiny_DFP/1.3.172/atdf/ATtiny13A.atdf  -o include/zoal/mcu/attiny13a.hpp
node index.js -f attiny -m ../zoal-mcu-metadata/ATtiny_DFP/1.3.172/atdf/ATtiny85.atdf  -o include/zoal/mcu/attiny85.hpp
node index.js -f attiny -m ../zoal-mcu-metadata/ATtiny_DFP/1.3.172/atdf/ATtiny2313A.atdf -o include/zoal/mcu/attiny2313a.hpp
# STM32
node index.js -f stm32 -m ~/STM32CubeMX/db/mcu/STM32F030F4Px.xml -s ../stm32/STM32F0x0.svd -o ./include/zoal/mcu/stm32f030f4px.hpp
node index.js -f stm32 -m ~/STM32CubeMX/db/mcu/STM32F103C\(8-B\)Tx.xml -s ../zoal-mcu-metadata/stm32/STM32F103.svd -o ./include/zoal/mcu/stm32f103c8tx.hpp
node index.js -f stm32 -m ~/STM32CubeMX/db/mcu/STM32F303R\(D-E\)Tx.xml -s ../zoal-mcu-metadata/stm32/STM32F303.svd -o ./include/zoal/mcu/stm32f303retx.hpp
node index.js -f stm32 -m ~/STM32CubeMX/db/mcu/STM32F303V\(B-C\)Tx.xml -s ../zoal-mcu-metadata/stm32/STM32F303.svd -o ./include/zoal/mcu/stm32f303vctx.hpp
node index.js -f stm32 -m ~/STM32CubeMX/db/mcu/STM32F401C\(B-C\)Ux.xml -s ../zoal-mcu-metadata/stm32/STM32F401.svd -o ./include/zoal/mcu/stm32f401ccux.hpp
