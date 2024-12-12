#include <stdio.h>
#include "sensorYL69.h"
#include "tempHumidity.h"
#include "yl69.h"
#include "esp_log.h"

static const char *TAG = "YL69_SENSOR";

// Configuración de sensores
static yl69_config_t sensor_configs[2] = {
    {
        .channel = ADC_CHANNEL_6,
        .unit = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
        .read_interval_ms = 1000,
        .sensor_id = 1,
        .queue = NULL,
        .priority = 1
    },
    {
        .channel = ADC_CHANNEL_7,
        .unit = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
        .read_interval_ms = 1000,
        .sensor_id = 2,
        .queue = NULL,
        .priority = 2
    }
};

void sensor_yl69_init(QueueHandle_t *queue_irrigation) {
    // asigna la cola de irrigation a cada sensor
    sensor_configs[0].queue = *queue_irrigation;
    sensor_configs[1].queue = *queue_irrigation;
    ESP_LOGI(TAG, "inicializar sensores YL69");
     // Inicializar cada sensor
     esp_err_t respYL69_1 = yl69_init(&sensor_configs[0]);
     if (respYL69_1 != ESP_OK) {
        ESP_LOGE(TAG, "Error al inicializar YL69_1: %d", respYL69_1);
     }
     esp_err_t respYL69_2 =yl69_init(&sensor_configs[1]);
     if (respYL69_2 != ESP_OK) {
        ESP_LOGE(TAG, "Error al inicializar YL69_2: %d", respYL69_2);
     }
   
}

void get_data_sensorYL69(void){
    //iniciar lecturas de sensores
    ESP_LOGI(TAG, "iniciar lectura de sensores YL69");
    yl69_start_reading(&sensor_configs[0]);
    yl69_start_reading(&sensor_configs[1]);
 
}