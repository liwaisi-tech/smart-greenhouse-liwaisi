#ifndef  ACTIONPAGE_H
#define ACTIONPAGE_H
#include "freertos/FreeRTOS.h"
#include <stdio.h>
#include "esp_err.h"
#include <stdio.h>
#include "tempHumidity.h"
#include "sensorDHT.h"
#include "yl69.h"

typedef struct {
    QueueHandle_t queue;
    int sensor_type;
    int sensor_id;
    yl69_reading_t readingYL69;
    dht22_reading_t readingDHT22;
} action_config_t;



void action_page_main (QueueHandle_t buffers[2], tempHumidity_t *data);
void receive_data_sensor(void *pvParameter);
esp_err_t receive_data_Yl69(void *pvParameter);
esp_err_t receive_data_DHT22(void *pvParameter);
#endif