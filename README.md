# SmartGrow Liwasi - Módulo Unifamiliar

## Descripción
Sistema IoT para control de microclima en invernadero familiar, diseñado para el cultivo de hortalizas en la región de los llanos. El sistema utiliza sensores de temperatura, humedad ambiental y humedad del suelo para automatizar sistemas de ventilación y riego.

## Características
- Monitoreo de temperatura y humedad ambiental usando sensores DHT22 (AM2301)
- Monitoreo de humedad del suelo usando sensores YL69
- Interfaz web para visualización de datos en tiempo real
- Control automático de sistemas de riego y ventilación
- Conectividad WiFi

## Componentes de Hardware
- ESP32 (microcontrolador principal)
- 2x DHT22/AM2301 (sensores de temperatura y humedad ambiental)
- 2x YL69 (sensores de humedad del suelo)
- Fuente de alimentación 3.3V

## Conexiones
### Sensores DHT22
- Sensor 1: GPIO33
- Sensor 2: GPIO32
- VCC: 3.3V
- GND: GND

### Sensores YL69
- Sensor 1: ADC1 Canal 6
- Sensor 2: ADC1 Canal 7
- VCC: 3.3V
- GND: GND

## Configuración del Software

### Prerrequisitos
- ESP-IDF v5.3.1
- Visual Studio Code con extensión ESP-IDF

### Instalación
1. Clonar el repositorio:

```bash
git clone https://github.com/briggitte-angrino/smart-greenhouse-liwaisi.git
```

2. Configurar el WiFi:
   - Modificar las credenciales WiFi en el archivo de configuración

3. Compilar y flashear:

```bash
idf.py build
idf.py -p [PUERTO] flash
```

## Estructura del Proyecto
```bash
smart-greenhouse-liwaisi/
├── components/
│   ├── sensorDHT/          # Componente para sensores DHT22
│   ├── sensorYL69/         # Componente para sensores de humedad del suelo
│   ├── yl69/                # Driver base para YL69
│   └── wifi/               # Configuración WiFi
│   └── webServer/          # Interfaz web
├── main/
│   └── sensor-iot-temp-hum.c  # Programa principal
│   └── index.html             # Interfaz web
```
