#include "Point.hpp"
#include "TFT_eSPI.h"

void Point::draw(TFT_eSPI *tft){
  tft->drawPixel(this->x, this->y, TFT_WHITE);
}


void Point::clear(TFT_eSPI *tft){
  tft->drawPixel(this->x, this->y, TFT_BLACK);
}


void Point::move(TFT_eSPI *tft){
  this->clear(tft);
  this->x++;
  this->draw(tft);
}