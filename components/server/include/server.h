#ifndef _SERVER_H
#define _SERVER_H

#include "esp_err.h"
#include "esp_http_server.h"
#include "moisture_sensor.h"

// Declaración de URI handlers (sin static)
esp_err_t home_get_handler(httpd_req_t *req);
esp_err_t data_sensor_get_handler(httpd_req_t *req);

// Declaración de URIs
extern const httpd_uri_t sensor;
extern const httpd_uri_t home;

// Inicialización del servidor web
void web_server_init();

#endif // _SERVER_H