#include "sensordispatcher.h"
#include <pressure.h>
#include <muxdriver.h>
#include <primitives.h>
#include <Arduino.h>

// Sesnor class instances
PressureSensor g_pressure_sensor;

bool sensor_dispatcher_init() {
    // Select mux channel 0 for the ADS1115
    if (!mux_select(0, 0)) {
        Serial.println("ERROR: Mux select failed!");
        return false;
    }
    delay(5);
    
    if (!g_pressure_sensor.init()) {
        Serial.println("ERROR: ADS1115 init failed!");
        return false;
    }
    
    return true;
}

bool sensor_read_dispatch(const SensorDesc &sensor, int32_t &data, int16_t &raw_adc) {
    switch (sensor.sensorType) {
        case SensorDesc::type::PRESSURE:
            return g_pressure_sensor.read(sensor, data, raw_adc);
        default:
            return false;
    }
}