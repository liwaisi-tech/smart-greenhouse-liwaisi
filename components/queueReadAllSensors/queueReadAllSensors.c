#include <stdio.h>
#include "queueReadAllSensors.h"

#define GPIO_SENSOR1 32
#define GPIO_SENSOR2 33
static const char *TAG = "queueReadAllSensors";
yl69_config_t config_Init_yl69(int sensor_id, int channel)
{
    yl69_config_t configYL69 = YL69_DEFAULT_CONFIG;
    configYL69.sensor_id = sensor_id;
    configYL69.channel = channel;
    return configYL69;
}

sensor_dht_config_t config_Init_dht(int pin, int sensor_type)
{
    return sensor_dht_config_t();
}

void queue_send_all_sensors(sensor_dht_config_t *dht_config, yl69_config_t *configYL69)
{
}
