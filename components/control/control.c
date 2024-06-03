#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "dht.h" 
#include <esp_log.h>

#define SENSOR_TYPE DHT_TYPE_AM2301
#define GPIO_DHT 14
QueueHandle_t buffer;//Objeto de la cola    
typedef struct 
{
 float temperature;
 float humidity;
} xMessage;

void sensor_ground(void *arg){

}


void send_data_climate(void *arg){
    xMessage sensor;
    //----------Variables de captura de temperatura y humedad---------//
    float temperature = 0, humidity = 0;
    while (1)
    {
        ESP_ERROR_CHECK(dht_read_float_data(SENSOR_TYPE, GPIO_DHT,  &temperature, &humidity));
        sensor.temperature = temperature;
        sensor.humidity = humidity;
        if (xQueueSend(buffer, &sensor, pdMS_TO_TICKS(100)) == pdTRUE) {
            printf("humidity: %f  temperatura: %f\n", humidity,temperature);
        }
         else
        {
            ESP_LOGE("Cola", "Item no recibido, timeout expiro!");
        }
    // Esperar un tiempo antes de leer nuevamente (por ejemplo, 1 segundos)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}

BaseType_t read_data_climate(xMessage *data){
    xMessage sensor;
    if (xQueueReceive(buffer, &sensor, pdMS_TO_TICKS(100)) == pdTRUE) {
        data->temperature = sensor.temperature;
        data->humidity = sensor.humidity;
        return pdTRUE;
    }
    else
    {
        return pdFALSE;
    }
}

void sensor_main()
{
    buffer = xQueueCreate(5, sizeof(xMessage));
    if (buffer == NULL) {
        ESP_LOGE("Cola", "Error al crear la cola");
        return;
    }

    xTaskCreatePinnedToCore(send_data_climate, "send_data_climate", 4096, NULL, 2, NULL, 0);
    //xTaskCreatePinnedToCore(sensor_ground, "sensor_ground", 4096, NULL, 1, NULL, 0);
}



