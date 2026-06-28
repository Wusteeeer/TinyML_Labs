Toolchain written for GD32VF103Cx8 custom KTH MCU for work in C++ that allows flashing via USB dfu.

Commands for building + flashing (linux):

**Clean**
rm -rf build

**Build**
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake

cmake --build build


**Flash (make sure mcu is in boot mode)**

cmake --build build --target flash
