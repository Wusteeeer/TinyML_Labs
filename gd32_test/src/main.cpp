#include "Led.hpp"


int main(){
  Led led;
  led.init();

  while(true){
    //led.toggle();
    led.on();
    for(int i = 0; i < 500000; i++);
    led.off();
    for(int i = 0; i < 500000; i++);
  }
}
