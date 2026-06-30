
extern "C"{
#include "pwm.h"
#include "lcd.h"
#include "../../vendor/GD32VF103_Firmware_Library_V1.7.0/Firmware/GD32VF103_standard_peripheral/Include/gd32vf103_timer.h"
#include "systick.h"
#include "subrout.h"
}

#include <math.h>
#include "Led.hpp"
#include "Point.hpp"

#define CLEAR_START 50
#define LCD_SIZE_X 160
#define LCD_SIZE_Y 80
#define DELAY 5000

int main(){
  //Led led;
  //led.init();

  t5omsi();
  
  Point points[170];
  int number_of_points = 0;
  int currentPoint = 0;
  int clearPos = 0;
  float x = 0;


  Lcd_SetType(LCD_INVERTED);
  Lcd_Init();
  LCD_Clear(BLACK);

  while(true){
    LCD_WR_Queue();

    if(t5expq()){

      
      if(number_of_points < LCD_SIZE_X){
        number_of_points++;
      }


      points[currentPoint].y = map(sin(x)*100, -100, 0, 100, LCD_SIZE_Y-1);
      points[currentPoint].x = currentPoint;

      points[currentPoint].draw();

      currentPoint++;
      if(currentPoint > LCD_SIZE_X){
        currentPoint = 0;
      }

      if(number_of_points > CLEAR_START){
        points[clearPos].clear();
        clearPos++;
        if(clearPos > LCD_SIZE_X){
          clearPos = 0;
        }
      }

      x += 0.1f;
      if(x >= 2.0*M_PI){
        x = 0;
      }
    }

    
 
    



  }
}
