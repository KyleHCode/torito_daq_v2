#include <Primitives.h>
#include <stdint.h>
#include <I2C.h>

bool i2c_write(uint8_t i2c_address, const uint8_t *data, size_t length, bool send_stop = true) {
    I2C.beginTransmission(i2c_address);
    I2C.write(data, length);
    uint8_t result = I2C.endTransmission(send_stop);
    return (result == 0);
}

bool i2c_read(uint8_t i2c_address, uint8_t *buffer, size_t length) {
    I2C.requestFrom(i2c_address, length);
    if (I2C.available() < length) {
        return false;
    }
    for (size_t i = 0; i < length; i++) {
        buffer[i] = I2C.read();
    }
    return true;
}

bool i2c_write_read(uint8_t i2c_address, uint8_t reg, uint8_t *buffer, size_t length) {
    if (!i2c_write(i2c_address, &reg, 1, false)) {
        return false;
    }
    return i2c_read(i2c_address, buffer, length);
}