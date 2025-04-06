# Componente ESP32-YL69
Este es un componente para ESP32 que permite la lectura de sensores de humedad del suelo YL-69 o Sensores capacitivos de salida analogica utilizando el framework ESP-IDF.

## 🎯 ¿Qué podemos hacer con este componente?
Por ejemplo podemos medir qué tan húmeda está la tierra de una planta usando un sensor y mostrar el resultado en porcentaje (0% = muy seco, 100% = muy húmedo).

## 📝 Lo que necesitas
- ESP32
- Sensor YL-69 (o sensor de humedad del suelo)
- Cable USB
- Computadora con ESP-IDF instalado

## 🔧 Conexiones
1. Conecta el sensor YL-69 a tu ESP32:
   - VCC → 3.3V
   - GND → GND
   - A0 → GPIO34 (ADC)

## Diagrama de Conexión
![Image](https://github.com/user-attachments/assets/9ce73af3-a50f-49df-8227-d89850f0b594)

## 💻 Pasos para usar el código

### 1️⃣ Crear tu proyecto
1. Abre una terminal
2. Crea una carpeta para tu proyecto y muevete a él:
```bash
mkdir mi_proyecto_planta
cd mi_proyecto_planta
```
3. Despliega el framework de Espressif (ESP-IDF):
```bash
 ~/esp/esp-idf/export.sh
```
4. Crea un proyecto ESP-IDF y muevete a él:
```bash
idf.py create-project sensor_humedad
cd sensor_humedad
```

### 2️⃣ Agregar el componente
1. Crea una carpeta `components`:
```bash
mkdir components
cd components
```
2. Agrega`components` al archivo `CMakeLists.txt` raiz:
```c
set(EXTRA_COMPONENT_DIRS components)
```
3. Descarga este componente:
```c
git clone https://github.com/liwaisi-tech/esp32-yl69.git
```
4. Copia el componente en la carpeta `components` de tu proyecto.

### 3️⃣ Copia este código
Abre tu proyecto con el IDE que prefieres y reemplaza todo el contenido de `main/main.c` con este código:

```c
#include <stdio.h>
#include "yl69.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"

static const char *TAG = "Capacitive-sensor";
#define CHANNEL1 ADC_CHANNEL_6
void app_main(void)
{
    int humedad = 0; 
    while (1)
    {
        yl69_config_t cfg = YL69_DEFAULT_CONFIG;
        cfg.channel = CHANNEL1;
        cfg.sensor_type = TYPE_CAP;
        yl69_init(&cfg);
        yl69_read_percentage(cfg.channel,&humedad,cfg.sensor_type);
        ESP_LOGI(TAG, "Humedad: %d%%",humedad);
        vTaskDelay(pdMS_TO_TICKS(5000));
    } 
}

```

### 4️⃣ Compilar y cargar
1. Regresa a la carpeta principal:
```bash
cd ../..
```

2. Compila el proyecto:
```bash
idf.py build
```

3. Conecta tu ESP32 y carga el programa:
```bash
idf.py -p (PUERTO) flash monitor
```

Reemplaza (PUERTO) con tu puerto COM en Windows (ejemplo: COM3).
## 📊 ¿Qué significan los números?
- 0-20%: ¡Tu planta se muere! 🏜️🏜️
- 21-45%: ¡Tu planta tiene mucha sed! 🏜️
- 50-70%: La humedad es buena 👍
- 71-100%: La tierra está muy húmeda 💧

## 🔍 Solución de problemas
- Si no ves números en la pantalla, revisa las conexiones
- Si ves números extraños, asegúrate que el sensor esté bien conectado
- Si el programa no carga, verifica que seleccionaste el puerto correcto

¡Diviértete experimentando! 🚀
