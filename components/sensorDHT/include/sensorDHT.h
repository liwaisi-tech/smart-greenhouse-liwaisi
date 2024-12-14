#ifndef SENSORDHT_H
#define SENSORDHT_H
#include "freertos/FreeRTOS.h"

typedef struct {
    uint8_t sensor_id;
    float humidity;
    float temperature;
    uint32_t timestamp;
} dht22_reading_t;

typedef struct {
    int pin;
    QueueHandle_t queue;
    int sensor_type;
} sensor_config_t;


void sensor_dht22_main(QueueHandle_t *buffer);
esp_err_t read_data_sensor(sensor_config_t *config, dht22_reading_t *data);
void read_sensor_task(void *arg);
#endif