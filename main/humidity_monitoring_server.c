#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "string.h"
#include "moisture_sensor.h"
#include "server.h"
#include "wifi.h"
#include "nvs_flash.h"

static const char *TAG = "HumidityMonitor";

static esp_err_t init_wifi(void) {
  esp_err_t ret;
   // Inicializar WiFi primero
  ret = wifi_action_mode(true);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to initialize WiFi");
    return ret;
  }
  ESP_LOGI(TAG, "WiFi initialized successfully");
  return ret;
}

void app_main(void)
{
 // Inicializar NVS (Non-volatile storage) necesario para WiFi
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  // Inicializar WiFi
  ret = init_wifi();
  if (ret == ESP_OK) {
    // Inicializar el servidor web
    web_server_init();
  }
  else {
    ESP_LOGE(TAG, "Error al inicializar la interfaz WiFi");
  }

  ESP_LOGI(TAG, "System initialized and running");
       
}   
