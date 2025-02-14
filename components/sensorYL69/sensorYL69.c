#include <stdio.h>
#include "sensorYL69.h"
#include "yl69.h"
#include "esp_log.h"

static const char *TAG = "YL69_SENSOR";

typedef struct {
    yl69_config_t configyl69;
    yl69_reading_t reading;
} taskParams;

void read_data_yl69_task(void *pvParameter) {
    taskParams *params = (taskParams *) pvParameter;
    ESP_LOGI(TAG, "yl69_start_reading Sensor %d", params->configyl69.sensor_id);
    params->reading.sensor_id = params->configyl69.sensor_id;
    params->reading.humidity = yl69_read_percentage(params->configyl69.channel);
}

void read_yl69_init(yl69_config_t *configYL69) {
    yl69_reading_t readingyl69 = {0}; // Inicializar la estructura
    taskParams params = {
        .configyl69 = *configYL69,
        .reading = readingyl69
    };
    // Iniciar la lectura del sensor
    ESP_LOGI(TAG, "Iniciando la lectura del sensor YL69");
    esp_err_t ret = yl69_init(&params.configyl69);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Error al iniciar config del sensor: %d", ret);
    }
    BaseType_t ret2 = xTaskCreate(read_data_yl69_task, "read_data_yl69_task", 2048, &params, 1, NULL);
    if (ret2 != pdPASS) {
        ESP_LOGE(TAG, "Error al iniciar la lectura del sensor: %d", ret2);
    }
    configYL69.reading = params.reading;
}
