#include <stdio.h>
#include "server.h"
#include "esp_http_server.h"
#include "esp_log.h"
#include "moisture_sensor.h"

#define CHANNEL ADC_CHANNEL_6 // Pin GPIO34 (D34)
static const char *TAG = "Resistive-sensor";

// Importar archivo HTML que se convierte en binario durante la compilación
extern const char index_start[] asm("_binary_index_html_start");
extern const char index_end[] asm("_binary_index_html_end");

// Definición de URIs
const httpd_uri_t sensor = {
  .uri = "/sensor",
  .method = HTTP_GET,
  .handler = data_sensor_get_handler,
  .user_ctx = NULL
}; 

const httpd_uri_t home = {
  .uri = "/home",
  .method = HTTP_GET,
  .handler = home_get_handler,
  .user_ctx = NULL
};

// Handler para la página principal
esp_err_t home_get_handler(httpd_req_t *req) {
    const u_int32_t index_len = index_end - index_start;
    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, index_start, index_len);
    return ESP_OK;
}

// Handler para obtener datos del sensor
esp_err_t data_sensor_get_handler(httpd_req_t *req) {
    httpd_resp_set_hdr(req, "Content-Type", "application/json");
    char res[100];
    int humedad = 0;
    
    // Configurar y leer el sensor
    moisture_sensor_config_t cfg = SENSOR_DEFAULT_CONFIG;
    cfg.channel = CHANNEL;
    cfg.sensor_type = CONFIG_SOIL_SENSOR_TYPE; // Si tienes un sensor Capacitivo usa TYPE_CAP
    moisture_sensor_init(&cfg); 
    sensor_read_percentage(cfg.channel, &humedad, cfg.sensor_type);
    ESP_LOGI(TAG, "Humedad: %d%%", humedad);

    // Enviar respuesta JSON con el valor de humedad
    sprintf(res, "{\"humedadSuelo\":%d}", humedad);
    httpd_resp_send(req, res, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Inicialización del servidor web
void web_server_init() {
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_register_uri_handler(server, &sensor);
        httpd_register_uri_handler(server, &home);
        ESP_LOGI(TAG, "Servidor HTTP iniciado correctamente");
        return;
    }
    
    ESP_LOGE(TAG, "Error al iniciar servidor HTTP");
}