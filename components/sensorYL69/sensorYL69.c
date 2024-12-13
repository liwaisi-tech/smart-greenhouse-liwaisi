#include <stdio.h>
#include "sensorYL69.h"
#include "yl69.h"
#include "esp_log.h"

static const char *TAG = "YL69_SENSOR";
static TaskHandle_t read_task_handle = NULL;
// Configuración de sensores
static yl69_config_t sensor_configs[2] = {
    {
        .channel = ADC_CHANNEL_6,
        .unit = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
        .read_interval_ms = 3000,
        .sensor_id = 1,
        .queue = NULL,
        .priority = 1
    },
    {
        .channel = ADC_CHANNEL_7,
        .unit = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,
        .read_interval_ms = 4000,
        .sensor_id = 2,
        .queue = NULL,
        .priority = 1
    }
};

void sensor_yl69_init(QueueHandle_t *queue_irrigation) {
    // asigna la cola de irrigation a cada sensor
    sensor_configs[0].queue = *queue_irrigation;
    sensor_configs[1].queue = *queue_irrigation;

     // Inicializar cada sensor
     esp_err_t respYL69_1 = yl69_init(&sensor_configs[0]);
     if (respYL69_1 != ESP_OK) {
        ESP_LOGE(TAG, "Error al inicializar YL69_1: %d", respYL69_1);
     }
     ESP_LOGI(TAG, "inicializar yl69-sensor2");
     esp_err_t respYL69_2 =yl69_init(&sensor_configs[1]);
     if (respYL69_2 != ESP_OK) {
        ESP_LOGE(TAG, "Error al inicializar YL69_2: %d", respYL69_2);
     }
   
}


void get_data_sensorYL69(void) {
    // Iniciar la lectura del primer sensor
    int8_t priority = sensor_configs[0].priority;

    BaseType_t ret1 = xTaskCreatePinnedToCore (task_send_data_yl69, "yl69_task", 2048, &sensor_configs[0], priority , &read_task_handle,0);
    if (ret1 != pdPASS) {
        ESP_LOGE(TAG, "Error al iniciar la lectura del sensor 1: %d", ret1);
    }

    // Iniciar la lectura del segundo sensor
    priority = sensor_configs[1].priority;
     BaseType_t ret2 = xTaskCreatePinnedToCore (task_send_data_yl69, "yl69_task", 2048, &sensor_configs[1], priority , &read_task_handle,0);
    if (ret2 != pdPASS) {
        ESP_LOGE(TAG, "Error al iniciar la lectura del sensor 1: %d", ret2);
    }

}