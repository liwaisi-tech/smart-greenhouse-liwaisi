#include <freertos/FreeRTOS.h>

void sensor_yl69_init(QueueHandle_t *queue_irrigation);
void get_data_sensorYL69(void);
void task_send_data_yl69(void *pvParameter);

