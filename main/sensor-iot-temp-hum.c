#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "esp_http_server.h"
#include "network.h"
#include "sensorDHT.h"

extern const char index_start[] asm("_binary_index_html_start");
extern const char index_end[] asm("_binary_index_html_end");

//----------Variables de captura de temperatura y humedad---------//
float temp1= 0, hum1 = 0, temp2 = 0, hum2 = 0;
//------ Variables mock para enviar al index----------//
int humgerm;

void read_data_task(void *pvParameters) { 
    while (1) {
        Data data;
        if (read_data_climate(&data) == pdTRUE) {
            temp1 = data.temp1;
            hum1 = data.hum1;
            temp2 = data.temp2;
            hum2 = data.hum2;
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Ajusta el intervalo de lectura según sea necesario
    }
}
//-----------Handler obtenerdatos---------//
static esp_err_t data_sensor_get_handler(httpd_req_t *req) {
    
    httpd_resp_set_hdr(req, "Content-Type", "application/json");
    char res[100];
    if (hum1 < 70) {
        humgerm = 1; //"Riego aereo On";
    }
    else if (hum1> 70) {
        humgerm = 0; //"Riego aereo Off";
    }

    sprintf(res, "{ \"hum1\": %f, \"temp1\": %f, \"hum2\": %f, \"temp2\": %f,\"alarma\": %d}", hum1, temp1,hum2,temp2,humgerm);
    httpd_resp_send(req, res, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

//-------------------Handler Home------------------//
static esp_err_t home_get_handler(httpd_req_t *req) {

    const u_int32_t index_len = index_end - index_start;

    httpd_resp_set_type(req,"text/html");
    httpd_resp_send(req,index_start,index_len);
    return ESP_OK;
}
//-----------Uris--------------------///
static const httpd_uri_t sensor = {
  .uri = "/value",
  .method = HTTP_GET,
  .handler = data_sensor_get_handler
}; 

static const httpd_uri_t home = {
  .uri = "/viewsensor",
  .method = HTTP_GET,
  .handler = home_get_handler
};

//-----------Inicialización del servidor-----------//
void web_server_init() {
  httpd_handle_t server = NULL;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();

  if (httpd_start(&server, &config) == ESP_OK) {
    httpd_register_uri_handler(server, &sensor);
    httpd_register_uri_handler(server, &home);
    return;
  }
  printf("Error al iniciar servidor\n");
}



void app_main()
{
    if (init_interface() == ESP_OK){
        sensor_main(); //Crea tarea que escribe valores en la cola
        xTaskCreate(read_data_task, "read_data_task", 4096, NULL, 3, NULL);
        web_server_init();
    }

}
