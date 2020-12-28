# zoal
Zero Overhead Abstraction Layer

```
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=ToolchainAVR.cmake
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=ToolchainHost.cmake
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=ToolchainSTM32.cmake

ln -s ~/STM32Cube/Repository ./cube-repo
node index.js -f stm32 -m ~/STM32CubeMX/db/mcu/STM32F303V(B-C)Tx.xml -o stm32f303vctx.hpp
```

### CLion AVRDude External tools configuration
Direct upload using avrdude
* Name: AVRDude Upload
* Program: avrdude
* Arguments -p m328p -c usbasp -Uflash:w:$CMakeCurrentTargetName$:e
* Working dir: $CMakeCurrentBuildDir$

Upload through cmake custom target
* Name: CMake Upload
* Program: $CMakeCurrentExecutableName$
* Arguments --build $CMakeCurrentBuildDir$ --target $CMakeCurrentTargetName$-upload
* Working dir: $CMakeCurrentBuildDir$
