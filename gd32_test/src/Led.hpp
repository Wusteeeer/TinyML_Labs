#pragma once

class Led{
public:
  bool ledOn;
  
  Led(){
    ledOn = false;
  }

  void init();

  void on();

  void off();

  void toggle();
};
