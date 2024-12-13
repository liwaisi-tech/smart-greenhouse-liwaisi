#ifndef SENSORDHT_H
#define SENSORDHT_H
#include "freertos/FreeRTOS.h"

typedef struct {
    uint8_t sensor_id;
    float hum;
    float temp;
    uint32_t timestamp;
} dht22_reading_t;

typedef struct {
    int GPIO_SENSOR;
    int SENSOR_TYPE;
    QueueHandle_t queue;    
    uint8_t sensor_id; 
    uint8_t priority;
} dht22_config_t;

#define dht22_DEFAULT_CONFIG { \
    .GPIO_SENSOR = 32,        \
    .SENSOR_TYPE = DHT_TYPE_AM2301, \
    .queue = NULL, \
    .sensor_id = 0, \
    .prority = 2 \
}

void sensor_dht22_main(QueueHandle_t *buffer);
esp_err_t read_data_sensor(dht22_config_t *config, dht22_reading_t *data);
void read_sensor_task(void *arg);
#endif