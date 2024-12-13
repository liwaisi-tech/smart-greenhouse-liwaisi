#include "yl69.h"
#include "esp_log.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "YL69";
static adc_oneshot_unit_handle_t adc_handle;
static bool adc_initialized = false;
static TaskHandle_t read_task_handle = NULL;

#define SENSOR_MAX_VALUE 4095
#define SENSOR_MIN_VALUE 0
#define HUMIDITY_MAX 100
#define HUMIDITY_MIN 0

static int map_value(int value) {
    return (value - SENSOR_MIN_VALUE) * (HUMIDITY_MIN - HUMIDITY_MAX) / 
           (SENSOR_MAX_VALUE - SENSOR_MIN_VALUE) + HUMIDITY_MAX;
}

esp_err_t yl69_init(yl69_config_t *config) {
    ESP_LOGI(TAG, "yl69_init");  
    if (!adc_initialized) {
        // Inicializar ADC solo una vez
        adc_oneshot_unit_init_cfg_t init_config = {
            .unit_id = config->unit,
            .ulp_mode = false
        };
        ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));
        adc_initialized = true;
    }
    
    // Configurar el canal específico
    adc_oneshot_chan_cfg_t chan_config = {
        .atten = config->atten,
        .bitwidth = config->bitwidth
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, config->channel, &chan_config));
    return ESP_OK;
}

int yl69_read_raw(adc_channel_t channel) {
    if (!adc_initialized) {
        ESP_LOGE(TAG, "YL69 no inicializado");
        return -1;
    }

    int raw_value;
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, channel, &raw_value));
    return raw_value;
}

int yl69_read_percentage(adc_channel_t channel) {
    return map_value(yl69_read_raw(channel));
}

void task_send_data_yl69(void *pvParameter) {
    yl69_config_t *config = (yl69_config_t *)pvParameter; 

    while (1) { // Bucle infinito
        ESP_LOGI(TAG, "yl69_start_reading yl69Sensor %d", config->sensor_id);
        yl69_reading_t reading = {
            .sensor_id = config->sensor_id,
            .humidity = yl69_read_percentage(config->channel)
        };
        
        if (xQueueSend(config->queue, &reading, pdMS_TO_TICKS(1000)) != pdTRUE) {
            ESP_LOGW(TAG, "Cola llena, dato descartado sensor %d",config->sensor_id);
        }
        
        vTaskDelay(pdMS_TO_TICKS(config->read_interval_ms)); // Espera antes de la siguiente lectura
    }
}

esp_err_t yl69_stop_reading(void) {
    if (read_task_handle != NULL) {
        ESP_LOGI(TAG, "yl69_stop_reading");
        vTaskDelete(read_task_handle);
        read_task_handle = NULL;
    }
    return ESP_OK;
}
