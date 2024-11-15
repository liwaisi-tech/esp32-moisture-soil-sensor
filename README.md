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

![Diagrama de conexión básico]
(Aquí iría una imagen del diagrama)

## 💻 Pasos para usar el código

### 1️⃣ Crear tu proyecto
1. Abre una terminal
2. Crea una carpeta para tu proyecto:
```bash
mkdir mi_proyecto_planta
cd mi_proyecto_planta
```

3. Crea un proyecto ESP-IDF:
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

2. Descarga este componente:
```bash
git clone https://github.com/usuario/esp32-yl69.git
```
3. Copia el componente en la carpeta `components` de tu proyecto.

### 3️⃣ Copia este código
Abre tu proyecto con el IDE que prefieres y reemplaza todo el contenido de `main/main.c` con este código:

```c
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "yl69.h"
#include "esp_log.h"

void app_main(void)
{
    // Crear buzón para los mensajes del sensor
    QueueHandle_t sensor_queue = xQueueCreate(10, sizeof(int));
    
    // Configurar nuestro sensor
    yl69_config_t config = YL69_DEFAULT_CONFIG;
    config.queue = sensor_queue;
    
    // Encender el sensor
    yl69_init(&config);
    
    // Comenzar a tomar medidas
    yl69_start_reading();
    
    // Leer y mostrar la humedad
    int humedad;
    while (1) {
        if (xQueueReceive(sensor_queue, &humedad, portMAX_DELAY)) {
            printf("🌱 Humedad de la planta: %d%%\n", humedad);
        }
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

Reemplaza (PUERTO) con tu puerto COM en Windows (ejemplo: COM3) o el puerto en Linux/Mac (ejemplo: /dev/ttyUSB0)

## 📊 ¿Qué significan los números?
- 0-20%: ¡Tu planta tiene mucha sed! 🏜️
- 20-60%: La humedad es buena 👍
- 60-100%: La tierra está muy húmeda 💧

## 🔍 Solución de problemas
- Si no ves números en la pantalla, revisa las conexiones
- Si ves números extraños, asegúrate que el sensor esté bien conectado
- Si el programa no carga, verifica que seleccionaste el puerto correcto

¡Diviértete experimentando! 🚀
