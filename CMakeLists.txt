idf_component_register(
    SRCS "Moisture sensor.c"
    INCLUDE_DIRS "include"
    REQUIRES "driver" "esp_adc" "freertos"
)