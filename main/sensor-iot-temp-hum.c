#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "esp_http_server.h"
#include "sensorDHT.h"
#include "wifi.h"
#include "tempHumidity.h"
#include "sensorYL69.h"
#include "yl69.h"
#include "esp_log.h"
#include "nvs_flash.h"

#define QUEUE_SIZE 10  // Define el tamaño de la cola
QueueHandle_t buffer_irrigation;  // Declara la variable global para la cola de riego
QueueHandle_t buffer_ventilation;  // Declara la variable global para la cola de ventilacion 

extern const char index_start[] asm("_binary_index_html_start");
extern const char index_end[] asm("_binary_index_html_end");

//----------Variables de captura de temperatura y humedad---------//
float temp1= 0, hum1 = 0, temp2 = 0, hum2 = 0;
//------ Variables mock para enviar al index----------//
int humgerm;
// Definir el TAG para logging
static const char *TAG = "sensor-iot_temp_hum";

void recive_data_YL69_task(void *pvParameters) {
    while (1) {
        yl69_reading_t reading;
        if (xQueueReceive(buffer_irrigation, &reading, 0) == pdTRUE) {
            ESP_LOGI(TAG, "Dato recibido Sensor %d humedad suelo %d", reading.sensor_id, reading.humidity);
        }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Ajusta el intervalo de lectura según sea necesario
    }
}

void read_data_task(void *pvParameters) { 
    while (1) {
        dht22_reading_t data;
        if (xQueueReceive(buffer_ventilation, &data, 0) == pdTRUE) {
            ESP_LOGI(TAG, "Lectura exitosa* Sensor %d Temp: %f°C, Hum1: %f", data.sensor_id, data.temp, data.hum);
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

void log_startup_info() {
    ESP_LOGI(TAG, "Startup..");
    ESP_LOGI(TAG, "Free memory: %" PRIu32 " bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG, "IDF version: %s", esp_get_idf_version());
}

void app_main()
{
    log_startup_info();
     esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    ESP_ERROR_CHECK(wifi_init_sta());

    //create the irrigation queue buffer 
    buffer_irrigation = xQueueCreate(QUEUE_SIZE, sizeof(tempHumidity_t));
    if (buffer_irrigation == NULL) {
        ESP_LOGE(TAG, "Error al crear la cola para YL69");
        return;
    }
    //create the ventilation queue buffer 
    buffer_ventilation = xQueueCreate(QUEUE_SIZE, sizeof(tempHumidity_t));
    if (buffer_ventilation == NULL) {
        ESP_LOGE(TAG, "Error al crear la cola para ventilacion");
        return;
    }

    // inicializar lecturas de sensores YL69
    sensor_yl69_init(&buffer_irrigation);
    get_data_sensorYL69();
    //Crear tarea de recepción de los datos a la cola
    xTaskCreate(recive_data_YL69_task, "recive_data_YL69_task", 4096, NULL, 2, NULL);
    
    sensor_dht22_main(&buffer_ventilation); //Crea tarea que escribe valores dht en la cola
    xTaskCreate(read_data_task, "read_data_task", 4096, NULL, 3, NULL);   
    web_server_init();
}

