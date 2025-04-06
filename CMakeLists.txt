idf_component_register(
    SRCS "moisture_sensor.c"
    INCLUDE_DIRS "include"
    REQUIRES "driver" "esp_adc" "freertos"
)