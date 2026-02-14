#ifndef SENSORCONFIG_H
#define SENSORCONFIG_H

#include <SensorDesc.h>

#define SENSOR_COUNT 4

// Sensor table - configure your hardware here
const SensorDesc sensor_table[SENSOR_COUNT] = {
    // id, type, bus_id, mux_channel, i2c_address, adc_channel, period_ticks
    {
        .id = 0,
        .sensorType = SensorDesc::type::PRESSURE,
        .bus_id = 0,
        .mux_channel = 0,           // PCA9548A channel 0 (SD0/SC0)
        .i2c_address = 0x48,        // ADS1115 address
        .adc_channel = 0,           // ADS1115 input A0
        .period_ticks = 1           // Read every frame
    },
    {
        .id = 1,
        .sensorType = SensorDesc::type::PRESSURE,
        .bus_id = 0,
        .mux_channel = 0,
        .i2c_address = 0x48,
        .adc_channel = 1,           // ADS1115 input A1
        .period_ticks = 1
    },
    {
        .id = 2,
        .sensorType = SensorDesc::type::PRESSURE,
        .bus_id = 0,
        .mux_channel = 0,
        .i2c_address = 0x48,
        .adc_channel = 2,           // ADS1115 input A2
        .period_ticks = 1
    },
    {
        .id = 3,
        .sensorType = SensorDesc::type::PRESSURE,
        .bus_id = 0,
        .mux_channel = 0,
        .i2c_address = 0x48,
        .adc_channel = 3,           // ADS1115 input A3
        .period_ticks = 1
    }
};

#endif // SENSORCONFIG_H