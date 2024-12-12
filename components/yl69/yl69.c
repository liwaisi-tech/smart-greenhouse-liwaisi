#include "yl69.h"
#include "esp_log.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static const char *TAG = "YL69";
static adc_oneshot_unit_handle_t adc_handle = NULL;
static bool adc_initialized = false;
static QueueHandle_t sensor_queue = NULL;
static uint32_t read_interval = 1000;
static adc_channel_t adc_channel;
static yl69_config_t current_config_t;
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

int yl69_read_raw(void)
{
    if (!adc_initialized) {
        ESP_LOGE(TAG, "YL69 no inicializado");
        return -1;
    }

    int raw_value;
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, adc_channel, &raw_value));
    return raw_value;
}

int yl69_read_percentage(void) {
    return map_value(yl69_read_raw());
}

static void task_read_yl69(void *pvParameter)
{
    yl69_config_t *config = (yl69_config_t *)pvParameter;
    
    while (1) {
        yl69_reading_t reading = {
            .sensor_id = config->sensor_id,
            .humidity = yl69_read_percentage()
        };
        
        if (xQueueSend(config->queue, &reading, 0) != pdTRUE) {
            ESP_LOGW(TAG, "Cola llena, dato descartado");
        }
        vTaskDelay(pdMS_TO_TICKS(read_interval));
    }
}

esp_err_t yl69_start_reading(yl69_config_t *config) {
    ESP_LOGI(TAG, "yl69_start_reading");
     // Guardar configuración con la que llega.
    memcpy(&current_config_t, config, sizeof(yl69_config_t));
    adc_channel = config->channel;
    read_interval = config->read_interval_ms;

    BaseType_t ret = xTaskCreate(task_read_yl69, "yl69_task", 2048, &config, 1, &read_task_handle);
    ESP_LOGI(TAG, "ret: %d", ret);
    return (ret == pdPASS) ? ESP_OK : ESP_FAIL;
}

esp_err_t yl69_stop_reading(void) {
    if (read_task_handle != NULL) {
        ESP_LOGI(TAG, "yl69_stop_reading");
        vTaskDelete(read_task_handle);
        read_task_handle = NULL;
    }
    return ESP_OK;
}


esp_err_t yl69_stop_reading(void) {
    // Solo eliminar la unidad ADC si es el último sensor
    if (adc_initialized) {
        ESP_ERROR_CHECK(adc_oneshot_del_unit(adc_handle));
        adc_initialized = false;
    }
    
    sensor_queue = NULL;
    
    return ESP_OK;
}