#ifndef QUEUE_READ_ALL_SENSORS_H
#define QUEUE_READ_ALL_SENSORS_H

#include <freertos/FreeRTOS.h>
#include "sensorYL69.h"
#include "yl69.h"
#include "esp_log.h"
#include "sensorDHT.h"
#include "tempHumidity.h"

// Configuración de sensores.
yl69_config_t config_Init_yl69(int sensor_id, int channel);
sensor_dht_config_t config_Init_dht(int pin, int sensor_type);
void queue_send_all_sensors(sensor_dht_config_t *dht_config, yl69_config_t *configYL69);

#endif // QUEUE_READ_ALL_SENSORS_H