#include <math.h>
#include "SPI.h"
#include "TFT_eSPI.h"
#include "Point.hpp"

#include "sine_model_data.h"
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
//#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/testing/micro_test.h"
#include "tensorflow/lite/schema/schema_generated.h"
//#include "tensorflow/lite/version.h"

// Globals, used for compatibility with Arduino-style sketches.
namespace {
const tflite::Model* model = nullptr;
tflite::MicroInterpreter* interpreter = nullptr;
TfLiteTensor* input = nullptr;
TfLiteTensor* output = nullptr;
int inference_count = 0;

// Create an area of memory to use for input, output, and intermediate arrays.
// Finding the minimum value for your model may require some trial and error.
constexpr int kTensorArenaSize = 2 * 1024;
uint8_t tensor_arena[kTensorArenaSize];
}  // namespace


#define CLEAR_START 100
#define SCREEN_CLEAR_COUNT 3

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

unsigned long total = 0;
unsigned long tn = 0;

Point points[320];
int numberOfPoints = 0;
int currentPoint = 0;
int clearPos = 0;
int cycleCount = 0;
float x = 0;
const int kInferencesPerCycle = 320;
const float kXrange = 2.f * 3.14159265359f;


long map_f(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println(""); Serial.println("");
  Serial.println("Bodmer's TFT_eSPI library Test!"); 
 
  tft.init();

  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);


  // Map the model into a usable data structure. This doesn't involve any
  // copying or parsing, it's a very lightweight operation.
  model = tflite::GetModel(g_sine_model_data);

  // This pulls in all the operation implementations we need.
  // NOLINTNEXTLINE(runtime-global-variables)
  static tflite::MicroMutableOpResolver<4> resolver;
  resolver.AddFullyConnected();
  resolver.AddRelu();
  resolver.AddQuantize();
  resolver.AddDequantize();
  //resolver.addConv2D();

  // Build an interpreter to run the model with.
  static tflite::MicroInterpreter static_interpreter(
      model, resolver, tensor_arena, kTensorArenaSize);
  interpreter = &static_interpreter;

  // Allocate memory from the tensor_arena for the model's tensors.
  TfLiteStatus allocate_status = interpreter->AllocateTensors();

  // Obtain pointers to the model's input and output tensors.
  input = interpreter->input(0);
  output = interpreter->output(0);

  // Keep track of how many inferences we have performed.
  inference_count = 0;
}


void loop() {

  if(cycleCount >= SCREEN_CLEAR_COUNT){
    cycleCount = 0;
    tft.fillScreen(TFT_BLACK);
  }

  float position = static_cast<float>(inference_count) /
                   static_cast<float>(kInferencesPerCycle);
  float x_val = position * kXrange;

  if(numberOfPoints < TFT_HEIGHT){
    numberOfPoints++;
  }

  points[currentPoint].setX(currentPoint);
  points[currentPoint].setY(map_f(sin(x), -1, 1, 0, 240));

  points[currentPoint].draw(&tft);
  currentPoint++;

  if(currentPoint > TFT_HEIGHT-1){
    currentPoint = 0;
    cycleCount++;
  }

  if(numberOfPoints > CLEAR_START){
    points[clearPos].clear(&tft);
    clearPos++;
    if(clearPos > TFT_HEIGHT){
      clearPos = 0;
    }
  }

  // Place our calculated x value in the model's input tensor
  input->data.f[0] = x;

  // Run inference, and report any error
  TfLiteStatus invoke_status = interpreter->Invoke();

  // Read the predicted y value from the model's output tensor
  float y_val = output->data.f[0];

  tft.drawPixel(currentPoint, map_f(y_val, -1,1,0,240), TFT_RED);

  // put your main code here, to run repeatedly:
  /*tft.setCursor(0,0);
  tft.setTextColor(TFT_MAGENTA);
  tft.setTextSize(2);


  tft.println(F(" TFT_eSPI test"));*/
  x += 0.1f;
  if(x >= M_PI*2.0){
    x = 0;
  }
  inference_count += 1;
  if (inference_count >= kInferencesPerCycle) inference_count = 0;
  Serial.println(numberOfPoints);
  delay(50);
}
