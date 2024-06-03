#ifndef CONTROL_H
#define CONTROL_H
#include "freertos/FreeRTOS.h"

typedef struct {
    float temperature;
    float humidity;
} xMessage;

void sensor_main();
BaseType_t read_data_climate(xMessage *data);

#endif