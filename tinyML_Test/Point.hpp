#pragma once
#include "TFT_eSPI.h"
class Point{
public:
  int x, y;

  Point(){
    this->x = -1;
    this->y = -1;
  }
  
  Point(int x, int y){
    this->x = x;
    this->y = y;
  }

  void draw(TFT_eSPI *tft);
  void clear(TFT_eSPI *tft);
  void move(TFT_eSPI *tft);

  const void setX(int x){
    this->x = x;
  }

  const void setY(int y){
    this->y = y;
  }
};