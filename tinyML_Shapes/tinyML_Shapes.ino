
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

#define NR_SHAPES 3


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

  uint8_t screen[1024] = {0};
  int x_size = 1;
  int y_size = 1;
  bool change_x = true;
  
  int current_shape = 0;
  int shape_change = 0;
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
  randomSeed(analogRead(0));


}

void loop() {
  // put your main code here, to run repeatedly:

  if(shape_change >= NR_SHAPES){
    shape_change = 0;
    current_shape = (current_shape+1)%3;
  }

  if(current_shape == 0){
    generate_ellipse(screen, 16, 16, random(0,16), random(0,16));
  }else if(current_shape == 1){
    generate_rectangle(screen, random(0,16), random(0,16), random(16,32), random(16, 32));
  }else{
    generate_triangle(screen, random(0,32), random(0,32), random(0,32), random(0,32), random(0,32), random(0,32));
  }
  
  
  input32x32(screen, 1024, WIDTH/2,HEIGHT/2,TFT_BLACK,TFT_WHITE);

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

  if(change_x){
    x_size++;
  }else{
    y_size++;
  }

  change_x = !change_x;
  delay(1000);
  tft.fillScreen(TFT_BLACK);
  clear_screen(screen, 1024);
  shape_change++;

}

void input32x32(const uint8_t* data, int len, int x_offset, int y_offset, uint32_t bg_color, uint32_t fg_color){

  int x = 0;
  int y = 0;
  for(int i = 0; i < len; i++){
    uint8_t currentPixel = data[x + (y*32)];
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

void generate_ellipse(uint8_t* screen, int center_x, int center_y, float a, float b){
  float current_x = cos(0);
  float current_y = sin(0);
  float angle = 0;
  while(angle <= (M_PI*2)){
    float radius = (a*b)/sqrt(((b*cos(angle))*(b*cos(angle)))+((a*sin(angle))*(a*sin(angle))));
    int index_x = round((current_x*radius)+center_x);
    int index_y = round((current_y*radius)+center_y);
    if(index_x < 32 && index_x >= 0 && index_y < 32 && index_y >= 0){
      screen[index_x + (index_y*32)] = 1;
    }   
    current_x = cos(angle);
    current_y = sin(angle);
    angle += 0.001f;
  }
        
}

void clear_screen(uint8_t* screen, int len){
  for(int i = 0; i < len; i++){
    screen[i] = 0;
  }
}

float linear_interpolation(float start, float end, float t){
  return (1.0f-t)*start + t*end;
}

void generate_line(uint8_t* screen, float start_point_x, float start_point_y, float end_point_x, float end_point_y){
  float t = 0;

  while(t <= 1){
      int index_x = round(linear_interpolation(start_point_x, end_point_x, t));
      int index_y = round(linear_interpolation(start_point_y, end_point_y, t));
      if(index_x < 32 && index_x >= 0 && index_y < 32 && index_y >= 0)
        screen[index_x + (index_y*32)] = 1;
      t += 0.001;
  }
       
}

void generate_rectangle(uint8_t* screen, float top_left_x, float top_left_y, float bottom_right_x, float bottom_right_y){
  // Top Line
  generate_line(screen, top_left_x, top_left_y, bottom_right_x, top_left_y);
  // Bottom Line
  generate_line(screen, top_left_x, bottom_right_y, bottom_right_x, bottom_right_y);
  // Right Line
  generate_line(screen, top_left_x, top_left_y, top_left_x, bottom_right_y);
  // Left Line
  generate_line(screen, bottom_right_x, top_left_y, bottom_right_x, bottom_right_y);
}

void generate_triangle(uint8_t *screen,
                      float vertex_1_x, float vertex_1_y, 
                      float vertex_2_x, float vertex_2_y,
                      float vertex_3_x, float vertex_3_y){
  generate_line(screen, vertex_1_x, vertex_1_y, vertex_2_x, vertex_2_y);
  generate_line(screen, vertex_2_x, vertex_2_y, vertex_3_x, vertex_3_y);
  generate_line(screen, vertex_3_x, vertex_3_y, vertex_1_x, vertex_1_y);
}

