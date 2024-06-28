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
QueueHandle_t buffer;//Objeto de la cola    


void send_data_climate(void *arg){
    while (1)
    {

     //----------Variables de captura de temperatura y humedad---------//
     Data sensor;
        ESP_ERROR_CHECK(dht_read_float_data(SENSOR_TYPE, GPIO_SENSOR1,&sensor.hum1,  &sensor.temp1 ));
         vTaskDelay(pdMS_TO_TICKS(500));
        ESP_ERROR_CHECK(dht_read_float_data(SENSOR_TYPE, GPIO_SENSOR2,&sensor.hum2,  &sensor.temp2 ));
        if (xQueueSend(buffer, &sensor, pdMS_TO_TICKS(100)) == pdTRUE) {
            printf("humidity Sensor1: %f  temperatura Sensor1: %f\n",sensor.hum1,sensor.temp1);
            printf("humidity Sensor2: %f  temperatura Sensor2: %f\n",sensor.hum2,sensor.temp2);
            printf("---------------------------------------------------\n");
        }
         else
        {
            ESP_LOGE("Cola", "Item no recibido, timeout expiro!");
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

    xTaskCreatePinnedToCore(send_data_climate, "send_data_climate", 4096, NULL, 2, NULL, 0);
    //xTaskCreatePinnedToCore(sensor_ground, "sensor_ground", 4096, NULL, 1, NULL, 0);
}

