cmake_minimum_required(VERSION 3.20)

project(GD32CPP LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MCU_FLAGS
  -march=rv32imac_zicsr_zifencei
  -mabi=ilp32
)

add_compile_options(
  ${MCU_FLAGS}
  -Wall
  -Wextra
  -ffunction-sections
  -fdata-sections
)

add_link_options(
  ${MCU_FLAGS}
  -T${CMAKE_SOURCE_DIR}/linker/GD32VF103x8.lds
  -Wl,--gc-sections
  -nostartfiles
)

set(SDK ${CMAKE_SOURCE_DIR}/../vendor/GD32VF103_Firmware_Library_V1.7.0)


include_directories(
  src
  ${SDK}/Firmware
  ${SDK}/Firmware/GD32VF103_standard_peripheral
  ${SDK}/Firmware/GD32VF103_standard_peripheral/Include 
  ${SDK}/Firmware/RISCV/drivers
)

set(DRIVERS 
  
  ${SDK}/Firmware/GD32VF103_standard_peripheral/system_gd32vf103.c

  ${SDK}/Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_gpio.c
  ${SDK}/Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_rcu.c
  ${SDK}/Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_timer.c
  ${SDK}/Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_eclic.c
  ${SDK}/Firmware/GD32VF103_standard_peripheral/Source/gd32vf103_spi.c
)

set(STARTUP 
  
  ${SDK}/Firmware/RISCV/env_Eclipse/entry.S
  ${SDK}/Firmware/RISCV/env_Eclipse/start.S
  ${SDK}/Firmware/RISCV/env_Eclipse/init.c
  ${SDK}/Firmware/RISCV/env_Eclipse/handlers.c
  ${SDK}/Firmware/RISCV/drivers/n200_func.c
)

add_executable(firmware 
  ${STARTUP}
  ${DRIVERS}

  src/main.cpp 
  src/Led.cpp
  src/Point.cpp
  src/pwm.c
  src/lcd.c
  src/systick.c
  src/subrout.S
)

target_compile_definitions(firmware PRIVATE 
  GD32VF103
)

set(CMAKE_OBJCOPY "/opt/xpack/riscv-none-elf-gcc/bin/riscv-none-elf-objcopy")
set(CMAKE_SIZE "/opt/xpack/riscv-none-elf-gcc/bin/riscv-none-elf-size")

add_custom_command(TARGET firmware POST_BUILD
  
  COMMAND ${CMAKE_OBJCOPY}
    -O binary
    $<TARGET_FILE:firmware>
    ${CMAKE_CURRENT_BINARY_DIR}/firmware.bin 

  COMMAND ${CMAKE_SIZE}
    $<TARGET_FILE:firmware>
)

add_custom_target(flash 

  COMMAND 
  dfu-util
    -a 0
    -s 0x08000000:leave
    -D firmware.bin 

  DEPENDS firmware 
)
