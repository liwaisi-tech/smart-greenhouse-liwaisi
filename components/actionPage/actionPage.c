#include <stdio.h>
#include "actionPage.h"
#include "tempHumidity.h"
#include "sensorYL69.h"
#include "yl69.h"
#include "esp_log.h"

tempHumidity_t receive_data_YL69_task(void *arg){
    action_config_t; *config = (action_config_t *) arg;
    tempHumidity_t data;
    yl69_reading_t reading;
    if (xQueueReceive(config.queue, &reading, 0) == pdTRUE) {
        if (reading.sensor_id ==1){
            data.humGroud1 = reading.humidity;
        }
        else{
            data.humGroud2 = reading.humidity;
        }
       
        ESP_LOGI(TAG, "Dato recibido humedad suelo cajon %d = %d", reading.sensor_id, reading.humidity);      
    }
        // Esta es para afuera cuando el handler llame, cada segundo.
       // vTaskDelay(pdMS_TO_TICKS(1000)); // Ajusta el intervalo de lectura según sea necesario
}
