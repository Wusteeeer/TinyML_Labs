extern "C"{
#include "lcd.h"
#include "../../vendor/GD32VF103_Firmware_Library_V1.7.0/Firmware/GD32VF103_standard_peripheral/Include/gd32vf103_timer.h"
#include "systick.h"
#include "subrout.h"
}

#include <math.h>
#include "Point.hpp"
#include "sine_model_data.h"
#include "output_handler.hpp"
#include "constants.h"

#include "main_functions.hpp"
/*#include "../../vendor/tflm-tree/tensorflow/lite/micro/micro_interpreter.h"
#include "../../vendor/tflm-tree/tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "../../vendor/tflm-tree/tensorflow/lite/micro/system_setup.h"
#include "../../vendor/tflm-tree/tensorflow/lite/schema/schema_generated.h"
#include "../../vendor/tflm-tree/tensorflow/lite/c/common.h"*/


#define CLEAR_START 100
#define LCD_SIZE_X 160
#define LCD_SIZE_Y 80
#define DELAY 5000

namespace{

  Point points[170];
  int number_of_points = 0;
  int currentPoint = 0;
  int clearPos = 0;
  float x = 0;

  /*tflite::ErrorReporter* error_reporter = nullptr;
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* input = nullptr;
  TfLiteTensor* output = nullptr;
  int inference_count = 0;

  // Create an area of memory to use for input, output, and intermediate arrays.
  // Finding the minimum value for your model may require some trial and error.
  constexpr int kTensorArenaSize = 2 * 1024;

  alignas(16)
  uint8_t tensor_arena[kTensorArenaSize];*/
}

void setup(){
    t5omsi();
    
    Lcd_SetType(LCD_INVERTED);
    Lcd_Init();
    LCD_Clear(BLACK);
    // Map the model into a usable data structure. This doesn't involve any
    // copying or parsing, it's a very lightweight operation.
    //model = tflite::GetModel(g_sine_model_data);
    /*if (model->version() != TFLITE_SCHEMA_VERSION) {
        error_reporter->Report(
            "Model provided is schema version %d not equal "
            "to supported version %d.",
            model->version(), TFLITE_SCHEMA_VERSION);
        return;
    }*/

    // Build an interpreter to run the model with.
    
    /*
    static tflite::MicroInterpreter static_interpreter(
        model, resolver, tensor_arena, kTensorArenaSize, error_reporter);
    interpreter = &static_interpreter;

    // Allocate memory from the tensor_arena for the model's tensors.
    TfLiteStatus allocate_status = interpreter->AllocateTensors();
    if (allocate_status != kTfLiteOk) {
        error_reporter->Report("AllocateTensors() failed");
        return;
    }

    input = interpreter->input(0);
    output = interpreter->output(0);

    inference_count = 0;*/
}


void loop(){

    LCD_WR_Queue();

    /*float position = static_cast<float>(inference_count) /
                   static_cast<float>(kInferencesPerCycle);
    float x_val = position * kXrange;

    // Place our calculated x value in the model's input tensor
    input->data.f[0] = x_val;

    // Run inference, and report any error
    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
        error_reporter->Report("Invoke failed on x_val: %f\n",
                            static_cast<double>(x_val));
        return;
    }

    // Read the predicted y value from the model's output tensor
    float y_val = output->data.f[0];

    // Output the results. A custom HandleOutput function can be implemented
    // for each supported hardware target.
    HandleOutput(error_reporter, x_val, y_val);

    // Increment the inference_counter, and reset it if we have reached
    // the total number per cycle
    inference_count += 1;
    if (inference_count >= kInferencesPerCycle) inference_count = 0;
    */
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