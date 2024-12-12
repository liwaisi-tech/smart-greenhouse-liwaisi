#ifndef TEMPHUMIDITY_H
#define TEMPHUMIDITY_H
#include <freertos/FreeRTOS.h>

typedef struct
{
    float temperature1;
    float humidity1;
    float temperature2;
    float humidity2;
    float humGroud1;
    float humGroud2;
} tempHumidity_t;

char* convert_to_json_string(tempHumidity_t* data);


#endif // TEMPHUMIDITY_H
