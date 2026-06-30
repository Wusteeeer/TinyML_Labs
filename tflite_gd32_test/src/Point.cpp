extern "C"{
#include "lcd.h"
}

#include "Point.hpp"


void Point::draw(){
  LCD_DrawPoint(this->x, this->y, WHITE);
}


void Point::clear(){
  LCD_DrawPoint(this->x, this->y, BLACK);
}


void Point::move(){
  this->clear();
  this->x++;
  this->draw();
}
