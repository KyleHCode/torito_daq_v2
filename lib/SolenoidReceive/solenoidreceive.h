#ifndef SOLENOIDRECEIVE_H
#define SOLENOIDRECEIVE_H

#include <Arduino.h>
#include <Wire.h>

// SolenoidReceive: read a 2-byte status payload from another MCU over I2C.
// - Uses `Wire.requestFrom()` to fetch two bytes.
// - Returns a 16-bit bitmask (MSB-first: buf[0] = high byte).
class SolenoidReceive {
public:
    SolenoidReceive() = default;

    // Initialize with the remote MCU I2C address. Returns true if address looks valid.
    bool init(uint8_t i2c_address);

    // Read 2 bytes from the remote MCU into `out_state` (MSB-first).
    // Returns true on success; false if I2C read failed or not initialized.
    bool read(uint16_t &out_state);

private:
    uint8_t i2c_address_ = 0;
};

#endif // SOLENOIDRECEIVE_H
