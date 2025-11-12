/*
 * MinimalNunchuk.h
 * Minimal custom library for reading a Wii Nunchuk.
 * IMPORTANT: This library does NOT call Wire.begin().
 * The user must call Wire.begin(SDA, SCL) BEFORE calling nunchuk.begin().
 */

#ifndef MINIMAL_NUNCHUK_H
#define MINIMAL_NUNCHUK_H

#include <Wire.h>

class MinimalNunchuk {
public:
    // Public variables for direct access
    int joyX, joyY;
    bool buttonC, buttonZ;
    int accelX, accelY, accelZ;

    MinimalNunchuk() {
        // Constructor (empty)
    }

    // Initializes the communication (DOES NOT call Wire.begin())
    bool begin() {
        Wire.beginTransmission(0x52); // Nunchuk I2C address
        Wire.write(0xF0);
        Wire.write(0x55);
        Wire.endTransmission();
        delay(1);
        Wire.beginTransmission(0x52);
        Wire.write(0xFB);
        Wire.write(0x00);
        return Wire.endTransmission() == 0;
    }

    // Reads data from the Nunchuk
    bool update() {
        Wire.requestFrom(0x52, 6); // Request 6 bytes of data
        if (Wire.available() < 6) {
            return false; // Read error
        }

        uint8_t data[6];
        for (int i = 0; i < 6; i++) {
            data[i] = Wire.read();
        }

        // Send the command for the next read
        Wire.beginTransmission(0x52);
        Wire.write(0x00);
        Wire.endTransmission();
        delay(1);

        // Decode the data
        joyX = data[0];
        joyY = data[1];
        accelX = (data[2] << 2) | ((data[5] >> 2) & 0x03);
        accelY = (data[3] << 2) | ((data[5] >> 4) & 0x03);
        accelZ = (data[4] << 2) | ((data[5] >> 6) & 0x03);
        
        buttonC = !(data[5] & 0x02); // Inverted logic: 0 = pressed
        buttonZ = !(data[5] & 0x01); // Inverted logic: 0 = pressed

        return true;
    }
};

#endif
