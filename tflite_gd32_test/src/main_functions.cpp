extern "C"{
#include "lcd.h"
#include "../../vendor/GD32VF103_Firmware_Library_V1.7.0/Firmware/GD32VF103_standard_peripheral/Include/gd32vf103_timer.h"
#include "systick.h"
#include "subrout.h"
}

#include <math.h>
#include "Point.hpp"

#include "main_functions.hpp"
#include "../../vendor/generic-tflmicro/src/tensorflow/lite/micro/micro_interpreter.h"
/*#include "../../tflite-micro/tensorflow/lite/micro/micro_interpreter.h"
#include "../../tflite-micro/tensorflow/lite/micro/micro_log.h"
#include "../../tflite-micro/tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "../../tflite-micro/tensorflow/lite/micro/micro_profiler.h"
#include "../../tflite-micro/tensorflow/lite/micro/recording_micro_interpreter.h"
#include "../../tflite-micro/tensorflow/lite/micro/system_setup.h"
#include "../../tflite-micro/tensorflow/lite/schema/schema_generated.h"
#include "../../tflite-micro/tensorflow/lite/core/c/common.h"
*/

#define CLEAR_START 50
#define LCD_SIZE_X 160
#define LCD_SIZE_Y 80
#define DELAY 5000

namespace{
  Point points[170];
  int number_of_points = 0;
  int currentPoint = 0;
  int clearPos = 0;
  float x = 0;
}

void setup(){
    t5omsi();
    
    Lcd_SetType(LCD_INVERTED);
    Lcd_Init();
    LCD_Clear(BLACK);
}


void loop(){

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