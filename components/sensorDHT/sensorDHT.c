#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "dht.h" 
#include <esp_log.h>
#include "sensorDHT.h"


#define SENSOR_TYPE DHT_TYPE_AM2301
#define GPIO_SENSOR1 33
#define GPIO_SENSOR2 32
static const char *TAG = "SensorDHT";
QueueHandle_t buffer;//Objeto de la cola    


void send_data_climate(void *arg){
    while (1)
    {

     //----------Variables de captura de temperatura y humedad---------//
     Data sensor;
      esp_err_t ret1 = dht_read_float_data(SENSOR_TYPE, GPIO_SENSOR1,&sensor.hum1,  &sensor.temp1);
        if (ret1 == ESP_OK) {
            // Procesar datos
            ESP_LOGI(TAG, "Lectura exitosa: Temp1: %f°C, Hum1: %f", sensor.temp1, sensor.hum1);
        } else {
            ESP_LOGW(TAG, "Error en lectura DHT: %d", ret1);
            // Esperar antes de reintentar
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
         esp_err_t ret2 = dht_read_float_data(SENSOR_TYPE, GPIO_SENSOR2, &sensor.hum2, &sensor.temp2);
        if (ret2 == ESP_OK) {
            // Procesar datos
            ESP_LOGI(TAG, "Lectura exitosa: Temp2: %f°C, Hum2: %f", sensor.temp2, sensor.hum2);
        } else {
            ESP_LOGW(TAG, "Error en lectura DHT: %d", ret2);
            // Esperar antes de reintentar
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
    // Esperar un tiempo antes de leer nuevamente (por ejemplo, 1 segundos)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}

BaseType_t read_data_climate(Data *data){
    Data sensor;
    if (xQueueReceive(buffer, &sensor, pdMS_TO_TICKS(100)) == pdTRUE) {
        data->temp1 = sensor.temp1;
        data->hum1 = sensor.hum1;
        data->hum2 = sensor.hum2;
        data->temp2 = sensor.temp2;
        return pdTRUE;
    }
    else
    {
        return pdFALSE;
    }
}

void sensor_main()
{
    buffer = xQueueCreate(5, sizeof(Data));
    if (buffer == NULL) {
        ESP_LOGE("Cola", "Error al crear la cola");
        return;
    }

    xTaskCreate(send_data_climate, "send_data_climate", 4096, NULL, 3, NULL);
    //xTaskCreatePinnedToCore(sensor_ground, "sensor_ground", 4096, NULL, 1, NULL, 0);
}

