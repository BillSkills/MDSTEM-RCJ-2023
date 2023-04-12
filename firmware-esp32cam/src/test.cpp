#include <TensorFlowLite_ESP32.h>
#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "tensorflow/lite/c/common.h"
#include "../lib/tflite_models/east-text-detector.h"

#include "esp_log.h"
#include "esp_camera.h"
#include "image_util.h"
#include "ko_config.h"
#include "img_converters.h"

tflite::MicroErrorReporter micro_error_reporter;
tflite::ErrorReporter *error_reporter = &micro_error_reporter;
const tflite::Model *model = ::tflite::GetModel(east_text_model);
const int tensor_arena_size = 81 * 1024;
uint8_t tensor_arena[tensor_arena_size];
tflite::MicroInterpreter *interpreter = NULL;
TfLiteTensor *input;
constexpr int kNumCols = 96;
constexpr int kNumRows = 96;
constexpr int kPersonIndex = 1;
constexpr int kNotAPersonIndex = 2;
const float PersonScoreThreshold = 0.6;

const char *imageName = "esp32cam_gs";
const int imageWidth = kNumCols;
const int imageHeight = kNumRows;

const framesize_t FrameSize = FRAMESIZE_96X96;       // should agree with kNumCols and kNumRows
const pixformat_t PixelFormat = PIXFORMAT_GRAYSCALE; // should be grayscale
bool initialiseCamera();
camera_fb_t *captureImage(bool useFlash);
void releaseCapturedImage(camera_fb_t *fb);
bool cameraReady;

void init_stream()
{
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        error_reporter->Report("Model provided is schema version %d not equal to supported version %d.",
                               model->version(), TFLITE_SCHEMA_VERSION);
    }

    tflite::MicroMutableOpResolver<5> *micro_op_resolver = new tflite::MicroMutableOpResolver<5>();
    micro_op_resolver->AddAveragePool2D();
    micro_op_resolver->AddConv2D();
    micro_op_resolver->AddDepthwiseConv2D();
    micro_op_resolver->AddReshape();
    micro_op_resolver->AddSoftmax();

    tflite::MicroInterpreter interpreter(model, *micro_op_resolver, tensor_arena, tensor_arena_size, error_reporter);

    interpreter.AllocateTensors();
    TfLiteTensor *input = interpreter.input(0);
}

void loop()
{
    if (interpreter == NULL)
    {
        error_reporter->Report("Not Initialized!");
        return;
    }

    camera_fb_t *capturedImage = captureImage(false);
    if (capturedImage == NULL) {
        error_reporter->Report("Error: Camera capture failed");
        return;
    }

    const uint8_t* wall_data = capturedImage->buf;
    for (int i = 0; i < input->bytes; ++i) {
      input->data.int8[i] = wall_data[i] ^ 0x80;  // signed int8_t quantized ==> input images must be converted from unisgned to signed format
    }

    TfLiteStatus invoke_status = interpreter->Invoke();
    if (invoke_status != kTfLiteOk) {
      error_reporter->Report("Invoke failed");
    }
}