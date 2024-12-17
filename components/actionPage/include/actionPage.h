#ifndef  ACTIONPAGE_H
#define ACTIONPAGE_H
#include "esp_err.h"

typedef struct {
    int pin;
    QueueHandle_t queue;
    int sensor_type;
} action_config_t;

void action_page_main (QueueHandle_t *buffer);
tempHumidity_t receive_data_YL69_task(void *arg);
tempHumidity_t receive_data_DHT_task (void *pvParameter);

#endif 