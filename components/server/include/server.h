#ifndef _SERVER_H
#define _SERVER_H

#include "esp_err.h"
#include "esp_http_server.h"
#include "moisture_sensor.h"

// Declaración de URI handlers
static esp_err_t home_get_handler(httpd_req_t *req);
static esp_err_t data_sensor_get_handler(httpd_req_t *req);

// Declaración de URIs
static const httpd_uri_t sensor = {
  .uri = "/sensor",
  .method = HTTP_GET,
  .handler = data_sensor_get_handler,
  .user_ctx = NULL
}; 

static const httpd_uri_t home = {
  .uri = "/home",
  .method = HTTP_GET,
  .handler = home_get_handler,
  .user_ctx = NULL
};

// Inicialización del servidor web
void web_server_init();

#endif // _SERVER_H