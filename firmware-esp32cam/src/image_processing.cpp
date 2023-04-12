//
// Created by dears on 2023-04-01.
//
#include "tensorflow/lite/micro/all_ops_resolver.h"
#include "tensorflow/lite/micro/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/micro/kernels/micro_ops.h"
#include "tensorflow/lite/schema/schema_generated.h"
#include "../lib/tflite_models/east-text-detector.h"
#include "../lib/tflite_models/keras-ocr.h"
#include "tensorflow/lite/c/common.h"

#include "esp_log.h"
#include "esp_camera.h"
#include "image_util.h"
#include "ko_config.h"

static const char *TAG = "img_proc";

namespace
{
    tflite::ErrorReporter *error_reporter = nullptr;
    const tflite::Model *model = nullptr;
    tflite::MicroInterpreter *interpreter = nullptr;
    TfLiteTensor *model_input = nullptr;

    constexpr int kTensorArenaSize = 81 * 1024;
    uint8_t tensor_arena[kTensorArenaSize];
    float *model_input_buffer = nullptr;
}

int init_module(camera_fb_t *fb)
{

    tflite::MicroErrorReporter micro_error_reporter;
    tflite::ErrorReporter *error_reporter = &micro_error_reporter;

    const tflite::Model *model = ::tflite::GetModel(keras_ocr_model);
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        TF_LITE_REPORT_ERROR(error_reporter,
                             "Model provided is schema version %d not equal "
                             "to supported version %d.\n",
                             model->version(), TFLITE_SCHEMA_VERSION);
    }

    tflite::AllOpsResolver resolver;
    tflite::MicroInterpreter interpreter(model, resolver, tensor_arena,
                                         kTensorArenaSize, error_reporter);

    interpreter.AllocateTensors();
    model_input = interpreter.input(0);
    model_input_buffer = model_input->data.f;

    int img_size = 96 * 96 * 3;
    uint8_t *tmp_buffer = (uint8_t *)malloc(img_size);

    image_resize_linear(tmp_buffer, fb->buf, 320, 320, 3, fb->width, fb->height);

    for (int i = 0; i < img_size; i++)
    {
        model_input_buffer[i] = tmp_buffer[i] / 255.0f;
    }
    free(tmp_buffer);

    TfLiteStatus invoke_status = interpreter.Invoke();
    if (invoke_status != kTfLiteOk)
    {
        TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed\n");
    }

    TfLiteTensor *output = interpreter.output(0);

    const tflite::Model *model = ::tflite::GetModel(east_text_model);
    if (model->version() != TFLITE_SCHEMA_VERSION)
    {
        TF_LITE_REPORT_ERROR(error_reporter,
                             "Model provided is schema version %d not equal "
                             "to supported version %d.\n",
                             model->version(), TFLITE_SCHEMA_VERSION);
    }

    tflite::AllOpsResolver resolver;

    tflite::MicroInterpreter interpreter(model, resolver, tensor_arena,
                                         kTensorArenaSize, error_reporter);

    interpreter.AllocateTensors();

    TfLiteTensor *input = interpreter.input(0);

    input = output;

    TfLiteStatus invoke_status = interpreter.Invoke();
    if (invoke_status != kTfLiteOk)
    {
        TF_LITE_REPORT_ERROR(error_reporter, "Invoke failed\n");
    }

    TfLiteTensor* output = interpreter.output(0);

    int value = output->data.f[1];
    ESP_LOGI(TAG, "Label=%s", kCategoryLabels[value]);
}

void classifyImage(camera_fb_t *fb)
{

    int img_size = 320 * 320 * 3;
    uint8_t *tmp_buffer = (uint8_t *)malloc(img_size);

    ESP_LOGI(TAG, "TMP Buffer");
    image_resize_linear(tmp_buffer, fb->buf, 320, 320, 3, fb->width, fb->height);
    ESP_LOGI(TAG, "Image resized");

    ESP_LOGI(TAG, "Size: %d", sizeof(tmp_buffer));
    for (int i = 0; i < img_size; i++)
    {
        model_input_buffer[i] = tmp_buffer[i] / 255.0f;
    }
    free(tmp_buffer);

    ESP_LOGI(TAG, "Invoking interpreter");
    if (kTfLiteOk != interpreter->Invoke())
    {
        error_reporter->Report("Error");
    }

    ESP_LOGI(TAG, "Showing results");
    TfLiteTensor *output = interpreter->output(0);

    for (int i = 1; i < kCategory; i++)
    {
        ESP_LOGI(TAG, "Label=%s, Prob=%f", kCategoryLabels[i], output->data.f[i]);
    }

    esp_camera_fb_return(fb);
    fb = NULL;
    ESP_LOGI(TAG, "Free Image");
}