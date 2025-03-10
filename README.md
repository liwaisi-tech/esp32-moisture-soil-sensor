# Componente ESP32-YL69
Este es un componente para ESP32 que permite la lectura de sensores de humedad del suelo YL-69 o MH Series utilizando el framework ESP-IDF.

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

<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 800 500">
  <!-- Fondo -->
  <rect width="800" height="500" fill="#f8f9fa"/>
  
  <!-- ESP32 -->
  <rect x="100" y="150" width="200" height="280" rx="10" fill="#d1d1d1" stroke="#333" stroke-width="2"/>
  <text x="200" y="130" font-family="Arial" font-size="20" text-anchor="middle" font-weight="bold">ESP32</text>
  
  <!-- Pines de la ESP32 -->
  <circle cx="300" cy="200" r="5" fill="#333"/>
  <text x="280" y="205" font-family="Arial" font-size="14" text-anchor="end">3V3</text>
  
  <circle cx="300" cy="250" r="5" fill="#333"/>
  <text x="280" y="255" font-family="Arial" font-size="14" text-anchor="end">GND</text>
  
  <circle cx="300" cy="300" r="5" fill="#333"/>
  <text x="280" y="305" font-family="Arial" font-size="14" text-anchor="end">GPIO34</text>
  
  <!-- Módulo YL-38 -->
  <rect x="450" y="150" width="150" height="200" rx="5" fill="#4a9" stroke="#333" stroke-width="2"/>
  <text x="525" y="130" font-family="Arial" font-size="20" text-anchor="middle" font-weight="bold">YL-38</text>
  
  <!-- Pines del YL-38 -->
  <circle cx="450" cy="200" r="5" fill="#333"/>
  <text x="470" y="205" font-family="Arial" font-size="14">VCC</text>
  
  <circle cx="450" cy="250" r="5" fill="#333"/>
  <text x="470" y="255" font-family="Arial" font-size="14">GND</text>
  
  <circle cx="450" cy="300" r="5" fill="#333"/>
  <text x="470" y="305" font-family="Arial" font-size="14">DO</text>
  
  <circle cx="450" cy="350" r="5" fill="#333"/>
  <text x="470" y="355" font-family="Arial" font-size="14">AO</text>
  
  <!-- Sensor YL-69 -->
  <rect x="700" y="200" width="60" height="100" fill="#b5b5b5" stroke="#333" stroke-width="2"/>
  <line x1="710" y1="230" x2="750" y2="230" stroke="#333" stroke-width="2"/>
  <line x1="710" y1="250" x2="750" y2="250" stroke="#333" stroke-width="2"/>
  <line x1="710" y1="270" x2="750" y2="270" stroke="#333" stroke-width="2"/>
  <text x="730" y="180" font-family="Arial" font-size="16" text-anchor="middle" font-weight="bold">YL-69</text>
  
  <!-- Conector de YL-69 a YL-38 -->
  <circle cx="700" cy="220" r="5" fill="#333"/>
  <circle cx="700" cy="280" r="5" fill="#333"/>
  
  <!-- Conexiones entre módulos -->
  <!-- VCC -->
  <path d="M 300 200 L 450 200" fill="none" stroke="red" stroke-width="2.5"/>
  <text x="375" y="190" font-family="Arial" font-size="14" fill="red" text-anchor="middle">VCC (3.3V)</text>
  
  <!-- GND -->
  <path d="M 300 250 L 450 250" fill="none" stroke="black" stroke-width="2.5"/>
  <text x="375" y="240" font-family="Arial" font-size="14" text-anchor="middle">GND</text>
  
  <!-- AO -> GPIO34 -->
  <path d="M 450 350 L 350 350 L 350 300 L 300 300" fill="none" stroke="blue" stroke-width="2.5"/>
  <text x="375" y="370" font-family="Arial" font-size="14" fill="blue" text-anchor="middle">AO → GPIO34</text>
  
  <!-- Conexión YL-38 a YL-69 -->
  <path d="M 600 220 L 650 220 L 650 220 L 700 220" fill="none" stroke="orange" stroke-width="2.5"/>
  <path d="M 600 280 L 650 280 L 650 280 L 700 280" fill="none" stroke="purple" stroke-width="2.5"/>
  
  <!-- Etiquetas de conexión YL-38 a YL-69 -->
  <text x="650" y="210" font-family="Arial" font-size="12" text-anchor="middle">Pin 1</text>
  <text x="650" y="270" font-family="Arial" font-size="12" text-anchor="middle">Pin 2</text>
  
  <!-- Leyenda -->
  <rect x="100" y="450" width="600" height="30" fill="#e9ecef" stroke="#333" stroke-width="1"/>
  <text x="400" y="470" font-family="Arial" font-size="14" text-anchor="middle">
    La ESP32 lee los valores de humedad a través del pin GPIO34 conectado a la salida analógica (AO) del YL-38
  </text>
</svg>

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
#define CHANNEL2_Yl69 ADC_CHANNEL_6
void app_main(void)
{
    int humedad = 0; 
    while (1)
    {
    yl69_config_t cfg_YL69 = YL69_DEFAULT_CONFIG;
    cfg_YL69.channel = CHANNEL2_Yl69;
    yl69_init(&cfg_YL69);
    yl69_read_percentage(cfg_YL69.channel,&humedad);
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
- 0-20%: ¡Tu planta tiene mucha sed! 🏜️
- 60-75%: La humedad es buena 👍
- 80-100%: La tierra está muy húmeda 💧

## 🔍 Solución de problemas
- Si no ves números en la pantalla, revisa las conexiones
- Si ves números extraños, asegúrate que el sensor esté bien conectado
- Si el programa no carga, verifica que seleccionaste el puerto correcto

¡Diviértete experimentando! 🚀
