#include <math.h>
#include "SPI.h"
#include "TFT_eSPI.h"

#include "shape_model_data.h"
#include "test_data_cir.h"
#include <tensorflow/lite/micro/micro_mutable_op_resolver.h>
//#include "tensorflow/lite/micro/micro_error_reporter.h"
#include <tensorflow/lite/micro/micro_interpreter.h>
#include <tensorflow/lite/micro/testing/micro_test.h>
#include <tensorflow/lite/schema/schema_generated.h>

#define WIDTH 320
#define HEIGHT 240

namespace{
  const tflite::Model* model = nullptr;
  tflite::MicroInterpreter* interpreter = nullptr;
  TfLiteTensor* input = nullptr;
  TfLiteTensor* output = nullptr;
  int inference_count = 0;
  float o_scale = 0.0f;
  int32_t o_zero_point = 0;

  constexpr int kTensorArenaSize = 39800;
  uint8_t tensor_arena[kTensorArenaSize];
}

TFT_eSPI tft = TFT_eSPI();

void setup() {

  Serial.begin(115200);
  while(!Serial);
  Serial.println(""); Serial.println("");
  Serial.println("TinyML CNN Begin");

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  // put your setup code here, to run once:

  model = tflite::GetModel(g_shape_model_data);

  static tflite::MicroMutableOpResolver<9> resolver;
  resolver.AddQuantize();
  resolver.AddDequantize();
  resolver.AddRelu();
  resolver.AddSoftmax();
  resolver.AddFullyConnected();
  resolver.AddDepthwiseConv2D();
  resolver.AddConv2D();
  resolver.AddReshape();
  resolver.AddMaxPool2D();

  static tflite::MicroInterpreter static_interpreter(
    model, resolver, tensor_arena, kTensorArenaSize
  );
  interpreter = &static_interpreter;

  TfLiteStatus allocate_status = interpreter->AllocateTensors();

  input = interpreter->input(0);

  inference_count = 0;

  Serial.println(interpreter->arena_used_bytes());
  //Serial.println(input->type);



}

void loop() {
  // put your main code here, to run repeatedly:
  render32x32(WIDTH/2,HEIGHT/2,TFT_BLACK,TFT_WHITE);

  TfLiteStatus status = interpreter->Invoke();

  if(status != kTfLiteOk){
    Serial.println("Invoke failed!");
    return;
  }

  output = interpreter->output(0);

  float circle_score = output->data.f[0];
  float rectangle_score = output->data.f[1];
  float triangle_score = output->data.f[2];

  Serial.printf(
      "Circle: %f Rectangle: %f Triangle: %f\n",
      circle_score,
      rectangle_score,
      triangle_score);

  tft.setCursor(WIDTH/2, HEIGHT/3);

  if(circle_score > rectangle_score){
    tft.println("Circle");
  }else if(rectangle_score > triangle_score){
    tft.println("Rectangle");
  }else{
    tft.println("Triangle");
  }


}

void render32x32(int x_offset, int y_offset, uint32_t bg_color, uint32_t fg_color){

  int x = 0;
  int y = 0;
  for(int i = 0; i < cir_len; i++){
    uint8_t currentPixel = cir_data[x + (y*32)];
    input->data.f[x + (y*32)] = currentPixel;

    if(currentPixel){
      tft.drawPixel(x+x_offset,y+y_offset,fg_color);
    }else{
      tft.drawPixel(x+x_offset,y+y_offset,bg_color);
    }
    x++;
    if(x >= 32){
      y++;
      x = 0;
    }
  }

}
