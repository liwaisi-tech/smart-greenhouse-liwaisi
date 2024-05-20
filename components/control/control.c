#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "dht.h" 


#define SENSOR_TYPE DHT_TYPE_AM2301
#define GPIO_DHT 14

//-----------Task para el sensado--------// 
void sensorDHT(){
    //----------Variables de captura de temperatura y humedad---------//
    float temperature = 0, humidity = 0;
    while(1)
    {
        ESP_ERROR_CHECK(dht_read_float_data(SENSOR_TYPE, GPIO_DHT,  &humidity, &temperature));
         printf("humidity: %f  temperatura: %f\n", humidity,temperature);

        if (temperature < 20.0){
            printf("Pongase saco porque la temperatura actual es: %f\n", temperature);
        }

        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }

}