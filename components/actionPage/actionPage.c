#include <stdio.h>
#include "actionPage.h"

void func(void)
{
/* void get_data_YL69_task(void *pvParameters) {
    yl69_reading_t reading;
    tempHumidity_t data = {0};
    
    while (1) {
        ESP_LOGI(TAG, "YL69sensor leyendo...");
        // Leer datos de ambos sensores
        if (xQueueReceive(sensor_configs[0].queue, &reading, portMAX_DELAY)) {
            if (reading.sensor_id == 1) {
                data.humGroud1 = reading.humidity;
                ESP_LOGI(TAG, "Humedad tierra cajón 1: %f", data.humGroud1);
            } else if (reading.sensor_id == 2) {
                data.humGroud2 = reading.humidity;
                ESP_LOGI(TAG, "Humedad tierra cajón 2: %f", data.humGroud2);
            }
        }
        else{
            ESP_LOGE(TAG, "No se recibio datos de YL69");
        }
        
        // Aquí puedes agregar código para enviar data a otra cola si es necesario
        vTaskDelay(pdMS_TO_TICKS(1000)); // Pequeña pausa para evitar sobrecarga
    } */
}
