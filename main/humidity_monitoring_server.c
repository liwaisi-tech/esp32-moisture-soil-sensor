#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "moisture_sensor.h"
#include "server.h"
#include "wifi.h"

static const char *TAG = "HumidityMonitor";

void app_main(void)
{

     ESP_LOGI(TAG, "Iniciando aplicación de monitoreo de humedad");
    
    // Inicializar la conexión WiFi
    if (init_interface() == ESP_OK) {
        ESP_LOGI(TAG, "WiFi inicializado correctamente");
        
        // Inicializar el servidor web
        web_server_init();
    } else {
        ESP_LOGE(TAG, "Error al inicializar la interfaz WiFi");
    }
}