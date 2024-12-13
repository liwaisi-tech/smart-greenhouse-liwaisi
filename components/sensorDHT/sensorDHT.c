#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "dht.h" 
#include <esp_log.h>
#include "sensorDHT.h"

#define MAX_SENSORS 2 // Número máximo de sensores
#define GPIO_SENSOR_BASE 32 // Base para GPIO de sensores
static const char *TAG = "SensorDHT";
static TaskHandle_t read_task_handle = NULL;

esp_err_t read_data_sensor(dht22_config_t *config, dht22_reading_t *data) {
    esp_err_t ret = dht_read_float_data(config->SENSOR_TYPE, config->GPIO_SENSOR, &data->hum, &data->temp);
    if (ret != ESP_OK) {
        ESP_LOGW(TAG, "Error en lectura DHT: %d", ret);
        return ret;
    }
    data->sensor_id = config->sensor_id; // Asignar ID del sensor
    return ESP_OK;
}

void read_sensor_task(void *arg) {
    dht22_config_t *config = (dht22_config_t *)arg;
    dht22_reading_t data;

    while (1) {
        esp_err_t ret = read_data_sensor(config, &data);
        if (ret == ESP_OK) {
            // Enviar lectura a la cola
            if (xQueueSend(config->queue, &data, pdMS_TO_TICKS(100)) != pdTRUE) {
                ESP_LOGW(TAG, "Cola llena, dato descartado sensor %d", config->sensor_id);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(3000)); // Esperar antes de la siguiente lectura
    }
}

void sensor_dht22_main(QueueHandle_t *buffer) { 

    dht22_config_t sensor_configs[MAX_SENSORS];
    for (int i = 0; i < MAX_SENSORS; i++) {
        sensor_configs[i].GPIO_SENSOR = GPIO_SENSOR_BASE + i; // Asignar GPIO
        sensor_configs[i].sensor_id = i+1; // Asignar ID único
        sensor_configs[i].queue = (QueueHandle_t)buffer; // Asegúrate de que buffer sea del tipo correcto

        // Crear tarea para cada sensor
        ESP_LOGI(TAG, "iniciar la lectura del sensorDHT:");
        BaseType_t ret1 = xTaskCreatePinnedToCore (read_sensor_task, "read_sensor_task", 2048, &sensor_configs[i], 3 , &read_task_handle,1);
    if (ret1 != pdPASS) {
        ESP_LOGE(TAG, "Error al iniciar la lectura del sensorDHT: %d", ret1);
    }
    }
}

