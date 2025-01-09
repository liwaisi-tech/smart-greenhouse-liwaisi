#include "esp_log.h"
#include "actionPage.h"

static const char *TAG = "actionPage";
#define DHT22_TYPE 1
#define YL69_TYPE  2
#define SENSORID1  1
#define SENSORID2  2

static void process_YL69_data(action_config_t *config, tempHumidity_t *data) {
    if (config->sensor_id == SENSORID1) {
        data->humGroud1 = config->readingYL69.humidity;
    } else {
        data->humGroud2 = config->readingYL69.humidity;
    }
}

static void process_DHT22_data(action_config_t *config, tempHumidity_t *data) {
    if (config->sensor_id == SENSORID1) {
        data->temperature1 = config->readingDHT22.temperature;
        data->humidity1 = config->readingDHT22.humidity;
    } else {
        data->temperature2 = config->readingDHT22.temperature;
        data->humidity2 = config->readingDHT22.humidity;
    }
}

esp_err_t receive_data_DHT22(void *pvParameter) {
    action_config_t *config = (action_config_t *) pvParameter;
    if (xQueueReceive(config->queue, &config->readingDHT22, 0) == pdTRUE) {
        config->sensor_id = config->readingDHT22.sensor_id;
        ESP_LOGI(TAG, "Lectura exitosa Cajon %d Temp: %f°C, Hum: %f", config->readingDHT22.sensor_id, config->readingDHT22.temperature, config->readingDHT22.humidity);
        return ESP_OK;
    } else {
        return ESP_FAIL;
    }
}

esp_err_t receive_data_Yl69(void *pvParameter) {
    action_config_t *config = (action_config_t *) pvParameter;
    if (xQueueReceive(config->queue, &config->readingYL69, 0) == pdTRUE) {
        config->sensor_id = config->readingYL69.sensor_id;
        ESP_LOGI(TAG, "Dato recibido humedad suelo cajon %d = %d", config->readingYL69.sensor_id, config->readingYL69.humidity);
        return ESP_OK;
    } else {
        return ESP_FAIL;
    }
}

void action_page_main(QueueHandle_t buffers[2], tempHumidity_t *data) {
    action_config_t config;

    // Procesar datos del sensor YL69
    config.queue = buffers[0];
    config.sensor_type = YL69_TYPE;
    if (receive_data_Yl69(&config) == ESP_OK) {
        process_YL69_data(&config, data);
    }

    // Procesar datos del sensor DHT22
    config.queue = buffers[1];
    config.sensor_type = DHT22_TYPE;
    if (receive_data_DHT22(&config) == ESP_OK) {
        process_DHT22_data(&config, data);
    }
}