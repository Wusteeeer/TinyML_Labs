Toolchain written for GD32VF103Cx8 custom KTH MCU board for programming in C++ and USB dfu flashing. 

**Clean**
rm -rf build

**Build**
cmake -B build -G Ninja -DCMAKE_TOOLCHAIN_FILE=toolchain.cmake

cmake --build build

**Flash**

cmake --build build --target flash
