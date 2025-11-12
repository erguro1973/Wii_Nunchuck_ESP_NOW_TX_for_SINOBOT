# Wii_Nunchuck_ESP_NOW_TX
Wii Nunchuck ESP-NOW Transmitter based on Xiao ESP32C6 RiscV
# Transmisor Nunchuk ESP-NOW (XIAO ESP32-C6)

Este proyecto convierte un mando Nunchuk de Wii en un controlador inalámbrico de bajo consumo utilizando ESP-NOW. Está diseñado específicamente para la placa **Seeed Studio XIAO ESP32-C6**, aprovechando su antena externa y su factor de forma compacto.

Es ideal para controlar rovers, drones, robots o cualquier proyecto de RC que requiera un control analógico y botones.



## 🚀 Características

* **Comunicación Inalámbrica:** Utiliza ESP-NOW para una comunicación de baja latencia y bajo consumo, sin necesidad de conectarse a una red WiFi.
* **Hardware Específico:** Optimizado para la placa XIAO ESP32-C6.
* **Antena Externa:** Configurado para usar el conector de antena U.FL (externa) para un alcance máximo.
* **Pines I2C Personalizados:** Utiliza los pines `D4 (GPIO 22)` y `D5 (GPIO 23)` para I2C, evitando conflictos con el puerto serie USB de la C6.
* **Bajo Consumo:** Diseñado para funcionar con una batería LiPo. *(Ver nota importante sobre alimentación)*.

---

## 🛠️ Componentes Hardware

* **Controlador:** Seeed Studio XIAO ESP32-C6
* **Mando:** Un Nunchuk de Wii (original o compatible).
* **Conector:** Adaptador de Nunchuk (o cables soldados directamente).
* **Alimentación:** Batería LiPo de 3.7V (ej. 500mAh).
* **Componente Crítico:** 1x Condensador Electrolítico de **220uF a 1000uF** (para estabilizar la alimentación de la batería).

---

## 🔌 Cableado

El Nunchuk usa I2C para comunicarse. En la XIAO C6, usamos pines no conflictivos:

| Nunchuk (Cable) | Pin XIAO (Función) | Pin Físico |
| :--- | :--- | :--- |
| **GND** (Blanco) | `GND` | `GND` |
| **+3.3V** (Rojo) | `3V3` | `3V3` |
| **SDA** (Verde) | `GPIO 23` | `D5` |
| **SCL** (Amarillo) | `GPIO 22` | `D4` |

### 🔋 ¡Nota Importante sobre Alimentación (Batería)!

El ESP32 consume picos de alta corriente (300-400mA) al transmitir por WiFi/ESP-NOW. Una batería LiPo no puede manejar estos picos instantáneos, lo que provoca una caída de voltaje (*brownout*) y reinicia el chip.

**SOLUCIÓN OBLIGATORIA:** Debes soldar un **condensador electrolítico (220uF a 1000uF)** entre los pines `3.3V` y `GND` de la placa XIAO. Esto actúa como un depósito para suministrar energía durante esos picos.

* Pata **negativa (-) del condensador** ➔ Pin **GND**
* Pata **positiva (+) del condensador** ➔ Pin **3.3V**

---

## ⚙️ Configuración del Software

### 1. Entorno (Arduino IDE)

Asegúrate de tener el gestor de placas para ESP32 de Espressif instalado y selecciona "Seeed Studio XIAO ESP32C6" como tu placa.

### 2. Librerías

Este proyecto requiere una librería de Nunchuk personalizada (`MinimalNunchuk.h`) para permitir la inicialización manual de I2C en pines específicos.

* `WiFi.h` (incluida con ESP32)
* `esp_now.h` (incluida con ESP32)
* `Wire.h` (incluida)
* `MinimalNunchuk.h` (Debes añadir esta librería a tu proyecto)

### 3. Código

#### Definición del Receptor
En el archivo `.ino` principal, debes especificar la dirección MAC de tu dispositivo receptor:



```cpp
// --- Dirección MAC del Receptor ÚNICO ---
uint8_t receiver_mac[] = {0x68, 0xFE, 0x71, 0x16, 0xBD, 0x44};
