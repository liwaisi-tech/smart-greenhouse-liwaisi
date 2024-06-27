#ifndef SENSORDHT_H
#define SENSORDHT_H
#include "freertos/FreeRTOS.h"

typedef struct 
{
 float temp1;
 float hum1;
 float temp2;
 float hum2;
 float humGr1;
 float humGr2;
} Data;

void sensor_main();
BaseType_t read_data_climate(Data *data);
#endif