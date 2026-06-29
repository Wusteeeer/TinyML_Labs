extern "C"{
#include "../vendor/GD32VF103_Firmware_Library_V1.7.0/Firmware/GD32VF103_standard_peripheral/gd32vf103.h"
#include "../vendor/GD32VF103_Firmware_Library_V1.7.0/Firmware/GD32VF103_standard_peripheral/Include/gd32vf103_gpio.h"
#include "../vendor/GD32VF103_Firmware_Library_V1.7.0/Firmware/GD32VF103_standard_peripheral/Include/gd32vf103_rcu.h"
}

#include "Led.hpp"
#include "Board.hpp"


void Led::init(){
  rcu_periph_clock_enable(RCU_GPIOB);

  gpio_init(LED_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, LED_PIN);
}

void Led::on(){
  gpio_bit_set(LED_PORT, LED_PIN);
  this->ledOn = true;
}

void Led::off(){
  gpio_bit_reset(LED_PORT, LED_PIN);
  this->ledOn = false;
}

void Led::toggle(){
  //gpio_bit_toggle(LED_PORT, LED_PIN);
}
