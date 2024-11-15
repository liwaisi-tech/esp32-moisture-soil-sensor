/*
 * Componente de ESP32 
 * Lectura sensor de humedad del suelo YL-69 o HL-69 Driver
 * Copyright 2024, Briggitte Castañeda <btatacc@gmail.com>
 */
#ifndef YL69_H
#define YL69_H
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_err.h"

typedef struct {
    adc_channel_t channel;
    adc_bitwidth_t bitwidth;
    adc_atten_t atten;
    adc_unit_t unit;
    QueueHandle_t queue;  
    uint32_t read_interval_ms;     
} yl69_config_t;

#define YL69_DEFAULT_CONFIG { \
    .channel = ADC_CHANNEL_6,        \
    .bitwidth = ADC_BITWIDTH_12,     \
    .atten = ADC_ATTEN_DB_12,        \
    .unit = ADC_UNIT_1,              \
    .read_interval_ms = 1000,        \
    .queue = NULL                    \
}

esp_err_t yl69_init(const yl69_config_t *config);
esp_err_t yl69_deinit(void);
int yl69_read_raw(void);
int yl69_read_percentage(void);
esp_err_t yl69_start_reading(void);
esp_err_t yl69_stop_reading(void);

#endif // YL69_H