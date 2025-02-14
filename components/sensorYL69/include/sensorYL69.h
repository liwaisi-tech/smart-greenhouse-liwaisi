#ifndef SENSOR_YL69_H
#define SENSOR_YL69_H

#include "freertos/FreeRTOS.h"
#include "yl69.h"

void read_yl69_init(yl69_config_t *configYL69);
void read_data_yl69_task(void *pvParameter);

#endif // SENSOR_YL69_H

