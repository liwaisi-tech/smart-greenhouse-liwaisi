#include "sensorDHT.h"
#include "dht.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <esp_log.h>
#include "sensorDHT.h"

#define MAX_SENSORS 2
#define GPIO_SENSOR1 32
#define GPIO_SENSOR2 33
#define SENSOR_TYPE  DHT_TYPE_AM2301
static const char *TAG = "SensorDHT";

static sensor_config_t sensor_configs[MAX_SENSORS];

void read_sensor_task(void *arg) {
    sensor_config_t *config = (sensor_config_t *)arg;
    float humidity, temperature;

    while (1) {

        esp_err_t ret = dht_read_float_data(config->sensor_type, config->pin, &humidity, &temperature);
        if (ret == ESP_OK) {
            dht22_reading_t reading = {
                .sensor_id = config->pin, // Usar el pin como ID del sensor
                .humidity = humidity,
                .temperature = temperature
            };
            if (xQueueSend(config->queue, &reading, pdMS_TO_TICKS(100)) != pdTRUE) {
                ESP_LOGW(TAG, "Cola llena, dato descartado sensor en GPIO %d", config->pin);
            }
        } else {
            ESP_LOGE(TAG, "Error al leer el sensor en GPIO %d: %d", config->pin, ret);
        }
        vTaskDelay(pdMS_TO_TICKS(5000)); // Esperar 2 segundos antes de la siguiente lectura
    }
}

void sensor_dht22_main( QueueHandle_t *buffer1) {
    sensor_configs[0].pin = GPIO_SENSOR1;
    sensor_configs[0].queue = *buffer1;
    sensor_configs[0].sensor_type = SENSOR_TYPE; // Tipo de sensor

    sensor_configs[1].pin = GPIO_SENSOR2;
    sensor_configs[1].queue = *buffer1;
    sensor_configs[1].sensor_type = SENSOR_TYPE; // Tipo de sensor

    for (int i = 0; i < MAX_SENSORS; i++) {
        // Crear tarea para cada sensor
        BaseType_t ret = xTaskCreate(read_sensor_task, "read_sensor_task", 2048, &sensor_configs[i], 1, NULL);
        if (ret != pdPASS) {
            ESP_LOGE(TAG, "Error al crear la tarea para el sensor en GPIO %d", sensor_configs[i].pin);
        }
    }
}

