## Código de la Librería (`MinimalNunchuk.h`)

```cpp
/*
 * MinimalNunchuk.h
 * Librería mínima para leer un Nunchuk de Wii.
 * IMPORTANTE: Esta librería NO llama a Wire.begin().
 * El usuario debe llamar a Wire.begin(SDA, SCL) ANTES de llamar a nunchuk.begin().
 */

#ifndef MINIMAL_NUNCHUK_H
#define MINIMAL_NUNCHUK_H

#include <Wire.h>

class MinimalNunchuk {
public:
    // Variables públicas para acceso directo
    int joyX, joyY;
    bool buttonC, buttonZ;
    int accelX, accelY, accelZ;

    MinimalNunchuk() {
        // Constructor (vacío)
    }

    // Inicializa la comunicación (NO llama a Wire.begin())
    bool begin() {
        Wire.beginTransmission(0x52); // Dirección I2C del Nunchuk
        Wire.write(0xF0);
        Wire.write(0x55);
        Wire.endTransmission();
        delay(1);
        Wire.beginTransmission(0x52);
        Wire.write(0xFB);
        Wire.write(0x00);
        return Wire.endTransmission() == 0;
    }

    // Lee los datos del Nunchuk
    bool update() {
        Wire.requestFrom(0x52, 6); // Pedir 6 bytes de datos
        if (Wire.available() < 6) {
            return false; // Error de lectura
        }

        uint8_t data[6];
        for (int i = 0; i < 6; i++) {
            data[i] = Wire.read();
        }

        // Enviar el comando para la próxima lectura
        Wire.beginTransmission(0x52);
        Wire.write(0x00);
        Wire.endTransmission();
        delay(1);

        // Decodificar los datos
        joyX = data[0];
        joyY = data[1];
        accelX = (data[2] << 2) | ((data[5] >> 2) & 0x03);
        accelY = (data[3] << 2) | ((data[5] >> 4) & 0x03);
        accelZ = (data[4] << 2) | ((data[5] >> 6) & 0x03);
        
        buttonC = !(data[5] & 0x02);
        buttonZ = !(data[5] & 0x01);

        return true;
    }
};

#endif
