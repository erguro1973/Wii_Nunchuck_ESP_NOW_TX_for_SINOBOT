## Código del Transmisor (`Transmisor_Nunchuk.ino`)

```cpp
/*
 * TRANSMISOR NUNCHUK (XIAO ESP32-C6)
 * ----------------------------------
 * Placa: XIAO ESP32-C6
 * I2C: Pines D4 (GPIO 22) y D5 (GPIO 23)
 * Antena: Externa (U.FL)
 * Librería: "MinimalNunchuk.h" (personalizada)
 */

#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <Wire.h>
#include "MinimalNunchuk.h" // <-- Nuestra "mini-librería"

// --- Configuración I2C (Placa XIAO ESP32-C6 en pines D4/D5) ---
#define I2C_SDA_PIN 23 // Pin D5 (GPIO 23) cable verde
#define I2C_SCL_PIN 22 // Pin D4 (GPIO 22) cable amarillo

// --- Configuración de Antena (Específica de XIAO C6) ---
#define RF_SWITCH_ENABLE_PIN 3  // GPIO 3: Pin para habilitar el control del switch
#define ANTENNA_SELECT_PIN 14 // GPIO 14: Pin para controlar el RF switch
// Coméntala para usar la antena INTERNA (PCB)
#define USE_EXTERNAL_ANTENNA 

// --- Canal ESP-NOW ---
#define ESP_NOW_CHANNEL 1

// --- Dirección MAC del Receptor ÚNICO ---
uint8_t receiver_mac[] = {0x68, 0xFE, 0x71, 0x16, 0xBD, 0x44}; 

// --- Estructura de Datos (Igual que en el RX) ---
typedef struct {
  int x; // Joystick X (0-255)
  int y; // Joystick Y (0-255)
  bool c; // Botón C
  bool z; // Botón Z
  int accX; // Acelerómetro X
  int accY; // Acelerómetro Y
  int accZ; // Acelerómetro Z
} NunchukData;

NunchukData myData; // Paquete para enviar

// --- Objetos ---
MinimalNunchuk nunchuk; // <-- Objeto de nuestra nueva librería
esp_now_peer_info_t peerInfo;

// --- Callback de Envío ---
void OnDataSent(const wifi_tx_info_t *wifi_info, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("Error al enviar el paquete.");
  }
}
  
void setup() {
  Serial.begin(115200);
  delay(1000); 
  Serial.println("Iniciando Transmisor (XIAO C6, I2C en D4/D5)...");

  // --- Selección de Antena (Específica de XIAO C6) ---
  pinMode(RF_SWITCH_ENABLE_PIN, OUTPUT);
  digitalWrite(RF_SWITCH_ENABLE_PIN, LOW); 
  pinMode(ANTENNA_SELECT_PIN, OUTPUT);
#ifdef USE_EXTERNAL_ANTENNA
  digitalWrite(ANTENNA_SELECT_PIN, HIGH); 
  Serial.println("Usando antena EXTERNA.");
#else
  digitalWrite(ANTENNA_SELECT_PIN, LOW); 
  Serial.println("Usando antena INTERNA.");
#endif
  delay(10); 
  
  // --- Inicializa I2C en los pines que funcionan (22 y 23) ---
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN); 
  
  // --- Inicializa Nunchuk ---
  if (!nunchuk.begin()) {
    Serial.println("¡Error: Nunchuk no detectado en D4/D5!");
  } else {
    Serial.println("Nunchuk inicializado.");
  }
  
  // Inicializa Wi-Fi y ESP-NOW
  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_wifi_set_channel(ESP_NOW_CHANNEL, WIFI_SECOND_CHAN_NONE);
  esp_now_register_send_cb(OnDataSent);
  
  memcpy(peerInfo.peer_addr, receiver_mac, 6);
  peerInfo.channel = ESP_NOW_CHANNEL;  
  peerInfo.encrypt = false;
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Fallo al añadir el peer");
    return;
  }
  
  Serial.println("Transmisor Nunchuk listo.");
}
  
void loop() {
  
  if (!nunchuk.update()) {
    Serial.println("Nunchuk desconectado!");
    delay(100);
    return; 
  }
    
  // --- Lógica de envío Sencilla ---
  myData.x = nunchuk.joyX;
  myData.y = nunchuk.joyY;
  myData.c = nunchuk.buttonC;
  myData.z = nunchuk.buttonZ;
  myData.accX = nunchuk.accelX;
  myData.accY = nunchuk.accelY;
  myData.accZ = nunchuk.accelZ;
  
  esp_now_send(receiver_mac, (uint8_t *) &myData, sizeof(myData));
  
  delay(20); 
}
```
